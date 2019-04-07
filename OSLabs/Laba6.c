#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <string.h>
#include <time.h>

void Take(int semId, int Number)
{
    struct sembuf op;

    op.sem_op = -1;
    op.sem_flg = 0;
    op.sem_num = Number;

    semop(semId, &op, 1);
}

void Put(int semId, int Number)
{
    struct sembuf op;

    op.sem_op = 1;
    op.sem_flg = 0;
    op.sem_num = Number;

    semop(semId, &op, 1);
}

void DelSem(int semId)
{
    semctl(semId, IPC_RMID, 0);
}

void DelShm(int memId)
{
    shmctl(memId, IPC_RMID, 0);
}

int main(int argv, char* argc[])
{
    srand(time(NULL));

    if(argv < 1)
    {
        printf("Enough arguments\n");
        return -1;
    }

    int N = atoi(argc[1]);
    int MaxValue = atoi(argc[3]);
    int MinValue = atoi(argc[2]);


   // std::cin >> N >> MinValue >> MaxValue;
    int semId = semget(IPC_PRIVATE, N, 0600|IPC_CREAT);

    if(semId < 0)
    {
        printf("Error with semget()\n");
        return -1;
    }

    int memId = shmget(IPC_PRIVATE, N * sizeof(int), 0600|IPC_CREAT|IPC_EXCL);

    if(memId <= 0)
    {
        printf("Error with shmget()\n");
        return -1;
    }

    int* Array = (int *)shmat(memId, 0, 0);

    for(int i = 0; i < N; i++)
    {   
	Array[i] = rand() % MaxValue + MinValue;

        printf("%i: %i\n", semctl(semId, i, GETVAL, 0), Array[i]);
    }

    printf("\n");

    pid_t childID = fork();

    if(childID < 0)
    {
        printf("Error with fork()\n");
    }
    else if(childID == 0)
    {
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
		 Put(semId, i);
		 Put(semId, j);

		 sleep(1);
               	 if(Array[i] > Array[j])
		 {
			int t = Array[i];
                    	Array[i] = Array[j];
                    	Array[j] = t;
		 }

		 Take(semId, j);
		 Take(semId, i);
           }
        }
    }
    else
    {
        printf("\n======================================\n");

        int count = 0;

        while(waitpid(childID, NULL, WNOHANG) == 0)
        {
            count = count + 1;

            printf("New iteration: %i\n", count);
            printf("---------------------------------\n");

	    sleep(1);
            for(int i = 0; i < N; i++)
	    {
                if(semctl(semId, i, GETVAL) == 0)
                {
                    printf("Free: %i\n", Array[i]);
                }
                else if(semctl(semId, i, GETVAL) > 0)
                {
                    printf("In waiting: %i\n", Array[i]);
                }
            }
        }

        printf("=======================================\n");

        printf("++++++++++++++++++++++++++++++++++++++++\n");
        printf("Sorted array:\n");

        for(int i = 0 ; i < N; i++)
        {
            printf("%i ", Array[i]);
        }

        printf("\n++++++++++++++++++++++++++++++++++++++++\n");

        DelSem(semId);
        DelShm(memId);
    }

    return 0;
}

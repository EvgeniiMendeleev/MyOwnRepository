#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <linux/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <new>
#include <iostream>

#define FIFO "FiFO"

int main(int argc, char* argv[])
{
	int N = atoi(argv[1]);

	pid_t Process = fork();

	if(Process < 0)
	{
		std::cout << "Error" << std::endl;
	}
	else if(Process == 0)
	{
		int FileDesc = open(FIFO, O_RDWR);
		int* SortArray = new int[N];
		while(!read(FileDesc, SortArray, N * sizeof(int)));

		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < N; j++)
			{
				if(SortArray[i] > SortArray[j])
				{
					int t = SortArray[i];
					SortArray[i] = SortArray[j];
					SortArray[j] = t;
				}
			}
		}

		while(!write(FileDesc, SortArray, N * sizeof(int)));
		close(FileDesc);
		delete[] SortArray;
	}
	else
	{
		umask(0);
		mknod(FIFO, S_IFIFO|0666, 0);

		srand(time(NULL));
		
		int FileDesc = open(FIFO, O_RDWR);
		int* Array = new int[N];

		printf("=============================\n");

		for(int i = 0; i < N; i++)
		{
			Array[i] = rand() % 100;
			std::cout << Array[i] << std::endl;
		}

		printf("=============================\n");

		while(!write(FileDesc, Array, N * sizeof(int)));
		waitpid(Process, NULL, 0);
		while(!read(FileDesc, Array, N * sizeof(int)));

		printf("==============================\n");

		for(int i = 0; i < N; i++)
		{
			printf("%i\n", Array[i]);
		}

		printf("==============================\n");

		close(FileDesc);
		unlink(FIFO);
		delete[] Array;
	}

	return 0;
}

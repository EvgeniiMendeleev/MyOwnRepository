#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctime>
#include <algorithm>

using namespace std;

struct Mess
{
	int Numbers[4];
}; 

void* parentMainCode(void* msg)
{
	srand(time(NULL));

	int* msgId = reinterpret_cast<int*>(msg);
	struct Mess Message;
	struct Mess ReceivedMessage;

	cout << "===========================\n";
	for(unsigned short i = 0; i < 4; i++)
	{
		Message.Numbers[i] = rand() % 1000;
		cout << Message.Numbers[i] << " ";
	}
	cout << "\n===========================\n";

	msgsnd(*msgId, &Message, sizeof(Message), 0);

	cout << "===========================\n";
	for(unsigned short i = 0; i < 24; i++)
	{
		msgrcv(*msgId, &ReceivedMessage, sizeof(Message), 0, 0);
		
		cout << i + 1 << ": ";
		
		for(int j = 0; j < 4; j++)
		{
			cout  << ReceivedMessage.Numbers[j] << " ";
		}
		cout << endl;
	}
	cout << "===========================\n";

	msgctl(*msgId, IPC_RMID, NULL);
	pthread_exit(0);
}

void* childMainCode(void* msg)
{
	int* msgId = reinterpret_cast<int*>(msg);
	struct Mess ChildMessage;

	msgrcv(*msgId, &ChildMessage, sizeof(ChildMessage), 0, 0);
	sort(ChildMessage.Numbers, ChildMessage.Numbers + 4);

	do{
		msgsnd(*msgId, &ChildMessage, sizeof(ChildMessage), 0);

	}while(::next_permutation(ChildMessage.Numbers, ChildMessage.Numbers + 4));

	pthread_exit(0);
}

int main()
{
	int msgId = msgget(IPC_PRIVATE, 0600|IPC_CREAT);
	void* MsgId = &msgId;

	if(msgId < 0)
	{
		cout << "Error with msgget()\n";
	}

	pthread_t Parent, Child;

	pthread_create(&Parent, 0, parentMainCode, MsgId);
	pthread_create(&Child, 0, childMainCode, MsgId);

	pthread_join(Parent, NULL);
	pthread_join(Child, NULL);

	return 0;
}

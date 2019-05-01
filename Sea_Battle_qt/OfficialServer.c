#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <iostream>

//enum states {WaitingOfTurn, Turn, WaitingOfPlayer, WaitingOfConnection};

//-------Состояния-------
#define WaitingOfConnection 3
#define WaitingOfPlayer 2
#define WaitingOfTurn 0
#define Turn 1
//----------------------

//--------Команды---------
#define StopGame -1
//-----------------------

using namespace std;

struct Position
{
	int i, j;  		//y и x - координаты выстрела.
	bool isReady;		//Состояние клиента на момент боя.
};

int FieldOfFP[100];
int FieldOfSP[100];

void showFieldFP()
{
        int count = 0;
        for(int i = 0; i < 100; i++)
        {
                cout << FieldOfFP[i] << " ";
        	
		if(i == 10 * count + 9)
                {
                        ++count;
                        cout << endl;
                }

	}

	cout << endl;
}

void showFieldSP()
{
        int c = 0;
        for(int i = 0; i < 100; i++)
        {
                cout << FieldOfSP[i] << " ";

		if(i == 10 * c + 9)
		{
			++c;
			cout << endl;
		}
        }

	cout << endl;

}

void* DataFromFirstClient(void* FirstClient)
{
	int* firstPlayer = static_cast<int*>(FirstClient);
	recv(*firstPlayer, &FieldOfFP, 100 * sizeof(int), MSG_NOSIGNAL);
	
	showFieldFP();

	pthread_exit(0);
}

void* DataFromSecondClient(void* SecondClient)
{
	int* secondPlayer = static_cast<int*>(SecondClient);
	recv(*secondPlayer, &FieldOfSP, 100 * sizeof(int), MSG_NOSIGNAL);
	
	showFieldSP();

	pthread_exit(0);
}

int main()
{	
	int_least8_t stateFirstPlayer, stateSecondPlayer;

	//---------------Создание слушающего сокета---------------------
	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in MasterAddr;
	MasterAddr.sin_family = AF_INET;
	MasterAddr.sin_port = htons(25567);
	MasterAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(MasterSocket, (struct sockaddr*) (&MasterAddr), sizeof(MasterAddr));

	listen(MasterSocket, 1);
	//-------------------------------------------------------------
	
	//----------------Соединение клиентов---------------------
	
	int FirstPlayer = accept(MasterSocket, 0, 0);

	if(FirstPlayer > 0)
	{
		printf("First player connected!\n");
		//stateFirstPlayer = WaitingOfConnection;
		//send(FirstPlayer, &stateFirstPlayer, sizeof(int_least8_t), MSG_NOSIGNAL);
	}
	else
	{
		printf("Connection had failed!\n");
		return 0;
	}

	int SecondPlayer = accept(MasterSocket, 0, 0);

	if(SecondPlayer > 0)
	{
		printf("Second player connected!\n");
		//int_least8_t state = WaitingOfPlayer;
		//send(FirstPlayer, &state, sizeof(state), MSG_NOSIGNAL);
		//send(SecondPlayer, &state, sizeof(state), MSG_NOSIGNAL);
	}
	else
	{
		printf("Connection had failed!\n");
		return 0;
	}
	//------------------------------------------------------
	
	//-----------------Получение полей с кораблями-------------
	void* First = &FirstPlayer;
	void* Second = &SecondPlayer;

	pthread_t FirstThread, SecondThread;

	pthread_create(&FirstThread, 0, DataFromFirstClient, First);
	pthread_create(&SecondThread, 0, DataFromSecondClient, Second);

	pthread_join(FirstThread, 0);
	pthread_join(SecondThread, 0);
	//--------------------------------------------------------

	//---------------Процесс обмена данными-------------------

	while(1)
	{

	}

	//--------------------------------------------------------
	return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <iostream>

enum states {WaitingOfTurn, Turn, WaitingOfReadyPlayer, Ready, WaitingOfConnection, PlacingShips, NotConnection};

//--------Команды---------
#define StopGame -1
//-----------------------

using namespace std;

struct ResultOfShoot
{
	int i, j;			//Координты отображения выстрела.
	bool isBung			//Результат попадания.
};

struct RecvPackage
{
	int i, j;  			//Координаты выстрела.
	states StateOfClient;		//Состояние клиента на момент боя.
};

int FieldOfSP[100], FieldOfFP[100];
int FirstPlayer, SecondPlayer;
states stateOfFirstPlayer, stateOfSecondPlayer;

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

void* DataFromFirstClient(void* NullData)
{
	stateOfFirstPlayer = PlacingShips;
	recv(FirstPlayer, &FieldOfFP, 100 * sizeof(int), MSG_NOSIGNAL);
	stateOfFirstPlayer = Ready;

	cout << "In First Thread: " << stateOfFirstPlayer << " " << stateOfSecondPlayer << "\n";

	if(stateOfSecondPlayer == PlacingShips)
	{
		stateOfFirstPlayer = WaitingOfReadyPlayer;
		send(FirstPlayer, &stateOfFirstPlayer, sizeof(stateOfFirstPlayer), MSG_NOSIGNAL);

		while(stateOfSecondPlayer != Ready);
	}

	stateOfFirstPlayer = Turn;
	send(FirstPlayer, &stateOfFirstPlayer, sizeof(stateOfFirstPlayer), MSG_NOSIGNAL);

	pthread_exit(0);
}

void* DataFromSecondClient(void* NullData)
{
	stateOfSecondPlayer = PlacingShips;
	recv(SecondPlayer, &FieldOfSP, 100 * sizeof(int), MSG_NOSIGNAL);
	stateOfSecondPlayer = Ready;
	
	cout << "In Second Thread: " << stateOfFirstPlayer << " " << stateOfSecondPlayer << "\n";

	if(stateOfFirstPlayer == PlacingShips)
	{
		stateOfSecondPlayer = WaitingOfReadyPlayer;
		send(SecondPlayer, &stateOfSecondPlayer, sizeof(stateOfSecondPlayer), MSG_NOSIGNAL);

		while(stateOfFirstPlayer != Ready);
	}
	
	stateOfSecondPlayer = WaitingOfTurn;
	send(SecondPlayer, &stateOfSecondPlayer, sizeof(stateOfSecondPlayer), MSG_NOSIGNAL);

	pthread_exit(0);
}

int main()
{
	stateOfFirstPlayer = stateOfSecondPlayer = NotConnection;

	cout << "Behind Threads 1: " << stateOfFirstPlayer << " " << stateOfSecondPlayer << "\n";

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
	
	FirstPlayer = accept(MasterSocket, 0, 0);

	if(FirstPlayer > 0)
	{
		printf("First player connected!\n");
		stateOfFirstPlayer = WaitingOfConnection;
		send(FirstPlayer, &stateOfFirstPlayer, sizeof(stateOfFirstPlayer), MSG_NOSIGNAL);
	}
	else
	{
		printf("Connection had failed!\n");
		return 0;
	}

	cout << "Behind Threads 2: " << stateOfFirstPlayer << " " << stateOfSecondPlayer << "\n";

	SecondPlayer = accept(MasterSocket, 0, 0);

	if(SecondPlayer > 0)
	{
		printf("Second player connected!\n");
		
		stateOfFirstPlayer = stateOfSecondPlayer = PlacingShips;
		
		send(FirstPlayer, &stateOfFirstPlayer, sizeof(stateOfFirstPlayer), MSG_NOSIGNAL);
		send(SecondPlayer, &stateOfSecondPlayer, sizeof(stateOfSecondPlayer), MSG_NOSIGNAL);
	}
	else
	{
		printf("Connection had failed!\n");
		return 0;
	}
	//------------------------------------------------------
	
	cout << "Behind Threads 3: " << stateOfFirstPlayer << " " << stateOfSecondPlayer << "\n";

	//-----------------Получение полей с кораблями-------------
	pthread_t FirstThread, SecondThread;
	void* NullData = NULL;

	pthread_create(&FirstThread, 0, DataFromFirstClient, NullData);
	pthread_create(&SecondThread, 0, DataFromSecondClient, NullData);

	pthread_join(FirstThread, 0);
	pthread_join(SecondThread, 0);
	//--------------------------------------------------------

	showFieldFP();
	cout << endl;
	showFieldSP();

	cout << "After threads: " << stateOfFirstPlayer << " " << stateOfSecondPlayer << "\n";

	//---------------Процесс обмена данными-------------------
	while(1)
	{
		struct RecvPackage InfoFromClient;
		struct ResultOfShoot Result;

		recv(FirstPlayer, &InfoFromClient, sizeof(RecvPackage), MSG_NOSIGNAL);

		if(InfoFromClient.StateOfClient != stateOfFirstPlayer)
		{
			cout << "Error! States of player don't equale!\n";
		}
	}
	//--------------------------------------------------------
	return 0;
}

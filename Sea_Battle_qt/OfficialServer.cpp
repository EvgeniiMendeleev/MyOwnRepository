#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <iostream>

using namespace std;

enum states {WaitingOfReadyPlayer, Ready, WaitingOfConnection, PlacingShips, NotConnection, WaitingOfTurn, Turn};
enum Msg_type {result_of_shot, enemy_shot, error};
enum ResultOfShot {not_hit, hit, kill};

struct Shot
{
	unsigned short  PosX;
	unsigned short  PosY;
};

struct Message
{
	Msg_type type;
	unsigned short PosX;
	unsigned short PosY;
	ResultOfShot Result;
};

int FirstPlayer, SecondPlayer;
int FieldOfSP[100], FieldOfFP[100];
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
		//send(*firstPlayer, &stateOfFirstPlayer, sizeof(stateOfFirstPlayer), MSG_NOSIGNAL);
		while(stateOfSecondPlayer != Ready);
	}

	stateOfFirstPlayer = Turn;
	//send(*firstPlayer, &stateOfFirstPlayer, sizeof(stateOfFirstPlayer), MSG_NOSIGNAL);

	//Информация о кораблях:
	//i = 0: число кораблей на поле.
	//i = 1: число подбитых палуб этого корабля.

	int OneShip = 4;
        int TwoShip[2] = {3, 0};
        int ThreeShip[2] = {2, 0};
        int FourShip[2] = {1, 0};

	while(OneShip || TwoShip[0] || ThreeShip[0] || FourShip[0])
	{
		Shot* InfoFromClient = new Shot;

		recv(FirstPlayer, InfoFromClient, sizeof(Shot), MSG_NOSIGNAL);

		if(stateOfFirstPlayer != Turn)
		{
			cout << "FirstPlayer, nizya! Sidi i jdi!" << endl;
		}
		else
		{
			if(FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] != -1)
			{
                                Message MsgForSP;
                                Message MsgForFP;

                                MsgForFP.type = result_of_shot;
                                MsgForSP.type = enemy_shot;

                                MsgForSP.PosY = MsgForFP.PosY = InfoFromClient->PosY;
                                MsgForSP.PosX = MsgForFP.PosX = InfoFromClient->PosX;


				if(FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] != 0)
				{
					cout << "First player bung in  " << FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] << endl;
					MsgForSP.Result = hit;
					MsgForFP.Result = hit;
				}
				else
				{
					cout << "First Player not bung!\n";
					MsgForSP.Result = not_hit;
					MsgForFP.Result = not_hit;
					stateOfFirstPlayer = WaitingOfTurn;
					stateOfSecondPlayer = Turn;
				}

				send(FirstPlayer, &MsgForFP, sizeof(MsgForFP), MSG_NOSIGNAL);
				send(SecondPlayer, &MsgForSP, sizeof(MsgForSP), MSG_NOSIGNAL);
			}
		}

		delete InfoFromClient;
	}

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
		//send(*secondPlayer, &stateOfSecondPlayer, sizeof(stateOfSecondPlayer), MSG_NOSIGNAL);

		while(stateOfFirstPlayer != Ready);
	}
	
	stateOfSecondPlayer = WaitingOfTurn;
	//send(*secondPlayer, &stateOfSecondPlayer, sizeof(stateOfSecondPlayer), MSG_NOSIGNAL);

	//Информация о кораблях:
	//i = 0: число кораблей на поле.
	//i = 1: число подбитых палуб этого корабля.
	int OneShip = 4;
	int TwoShip[2] = {3, 0};
	int ThreeShip[2] = {2, 0};
	int FourShip[2] = {1, 0};

	while(OneShip || TwoShip[0] || ThreeShip[0] || FourShip[0])
	{
		Shot* InfoFromClient = new Shot;

		recv(SecondPlayer, InfoFromClient, sizeof(Shot), MSG_NOSIGNAL);

		if(stateOfSecondPlayer != Turn)
		{
			cout << "SecondPlayer, nizya! Sidi i jdi!" << endl;
		}
		else
		{
			if(FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] != -1)
			{
				if(FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] != 0)
				{
					cout << "Second player bung in " << FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] << endl;
				}
				else
				{
					cout << "Second player not bung!\n";
					stateOfSecondPlayer = WaitingOfTurn;
					stateOfFirstPlayer = Turn;
				}
			}
		}

		delete InfoFromClient;
	}

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
		//send(FirstPlayer, &stateOfFirstPlayer, sizeof(stateOfFirstPlayer), MSG_NOSIGNAL);
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
		
		//send(FirstPlayer, &stateOfFirstPlayer, sizeof(stateOfFirstPlayer), MSG_NOSIGNAL);
		//send(SecondPlayer, &stateOfSecondPlayer, sizeof(stateOfSecondPlayer), MSG_NOSIGNAL);
	}
	else
	{
		printf("Connection had failed!\n");
		return 0;
	}
	//------------------------------------------------------
	
	cout << "Behind Threads 3: " << stateOfFirstPlayer << " " << stateOfSecondPlayer << "\n";

	//----------------------Процесс игры----------------------
	void* Null = NULL;
	
	pthread_t FirstThread, SecondThread;

	pthread_create(&FirstThread, 0, DataFromFirstClient, Null);
	pthread_create(&SecondThread, 0, DataFromSecondClient, Null);

	pthread_join(FirstThread, 0);
	pthread_join(SecondThread, 0);
	//--------------------------------------------------------

	return 0;
}

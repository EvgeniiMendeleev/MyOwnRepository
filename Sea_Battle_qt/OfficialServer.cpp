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

struct RecvPackage
{
	int i, j;  			//Координаты выстрела.
};

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

void* DataFromFirstClient(void* FirstClient)
{
	int* firstPlayer = static_cast<int*>(FirstClient);

	stateOfFirstPlayer = PlacingShips;
	recv(*firstPlayer, &FieldOfFP, 100 * sizeof(int), MSG_NOSIGNAL);
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
	int* isBung;			//Результат выстрела

	while(OneShip || TwoShip[0] || ThreeShip[0] || FourShip[0])
	{
		struct RecvPackage* InfoFromFP = new RecvPackage;

		recv(*firstPlayer, InfoFromFP, sizeof(RecvPackage), MSG_NOSIGNAL);

		cout << "Yabadabaduuuuuuuuu!\n";

		if(stateOfFirstPlayer != Turn)
		{
			isBung = new int(-1);
			cout << "FirstPlayer, nizya. Sidi i jdi!\n";
			cout << "isBungFP = " << *isBung << endl;
			send(*firstPlayer, isBung, sizeof(int), MSG_NOSIGNAL);
		}
		else
		{
			cout << "First player are shooting in: " << InfoFromFP->i << " " << InfoFromFP->j << "\n";
			
			if(FieldOfSP[10 * InfoFromFP->i + InfoFromFP->j] != -1)
			{
				if(FieldOfSP[10 * InfoFromFP->i + InfoFromFP->j] != 0)
				{
					cout << "First player bang in " << FieldOfSP[10 * InfoFromFP->i + InfoFromFP->j] << "\n";
					isBung = new int(1);
				}
				else
				{
					isBung = new int (0);
					cout << "Not bang!\n";
					stateOfFirstPlayer = WaitingOfTurn;
					stateOfSecondPlayer = Turn;
				}
				
				send(*firstPlayer, isBung, sizeof(int), MSG_NOSIGNAL);
			}
			else
			{
				isBung = new int(-1);
				send(*firstPlayer, isBung, sizeof(int), MSG_NOSIGNAL);
			}
		}
		
		delete isBung;
		delete InfoFromFP;
	}

	pthread_exit(0);
}

void* DataFromSecondClient(void* SecondClient)
{
	int* secondPlayer = static_cast<int*>(SecondClient);
	stateOfSecondPlayer = PlacingShips;
	recv(*secondPlayer, &FieldOfSP, 100 * sizeof(int), MSG_NOSIGNAL);
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
	int* isBung;			//Результат выстрела.

	while(OneShip || TwoShip[0] || ThreeShip[0] || FourShip[0])
	{
		struct RecvPackage* InfoFromSP = new RecvPackage;

		recv(*secondPlayer, InfoFromSP, sizeof(RecvPackage), MSG_NOSIGNAL);

		if(stateOfSecondPlayer != Turn)
		{
			isBung = new int(-1);
			cout << "SecondPlayer, nizya. Sidi i jdi!\n";
			cout << "isBungSP = " << (*isBung) << endl;
			send(*secondPlayer, isBung, sizeof(int), MSG_NOSIGNAL);
		}
		else
		{
			cout << "Second player are shooting in: " << InfoFromSP->i << " " << InfoFromSP->j << "\n";
			
			if(FieldOfFP[10 * InfoFromSP->i + InfoFromSP->j] != -1)
			{
				if(FieldOfFP[10 * InfoFromSP->i + InfoFromSP->j] != 0)
                        	{
                        	        cout << "Second player bang in " << FieldOfFP[10 * InfoFromSP->i + InfoFromSP->j] << "\n";
					isBung = new int(1);
				}
                        	else
                        	{
                                	cout << "Second player Not bang!\n";
                        		isBung = new int(0);
					stateOfFirstPlayer = Turn;
					stateOfSecondPlayer = WaitingOfTurn;
				}
				
				send(*secondPlayer, isBung, sizeof(int), MSG_NOSIGNAL);
			}
			else
			{
				isBung = new int(-1);
				send(*secondPlayer, isBung, sizeof(int), MSG_NOSIGNAL);
			}
		}
		
		delete isBung;
		delete InfoFromSP;
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
	
	int FirstPlayer = accept(MasterSocket, 0, 0);

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

	int SecondPlayer = accept(MasterSocket, 0, 0);

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
	pthread_t FirstThread, SecondThread;

	pthread_create(&FirstThread, 0, DataFromFirstClient, &FirstPlayer);
	pthread_create(&SecondThread, 0, DataFromSecondClient, &SecondPlayer);

	pthread_join(FirstThread, 0);
	pthread_join(SecondThread, 0);
	//--------------------------------------------------------

	return 0;
}

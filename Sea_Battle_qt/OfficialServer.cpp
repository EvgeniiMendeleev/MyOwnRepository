#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <iostream>

using namespace std;

enum states {WaitingOfReadyPlayer, WaitingOfConnection, Win, Lose, PlacingShips,NotConnection, Ready, WaitingOfTurn, Turn};
enum Msg_type {result_of_shot, enemy_shot, state_for_client, error};
enum ResultOfShot {not_hit, hit, kill};

struct Shot
{
	int16_t PosX;
	int16_t PosY;
};

struct Message
{
	int16_t type;
	int16_t PosX;
	int16_t PosY;
	int16_t Result;
};

int FirstPlayer, SecondPlayer;
int16_t FieldOfSP[100], FieldOfFP[100];
states stateOfFirstPlayer, stateOfSecondPlayer;

void showFP()
{
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			cout << FieldOfFP[10 * i + j] << " ";
		}

		cout << endl;
	}
}

void showSP()
{
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			cout << FieldOfSP[10 * i + j] << " ";
		}

		cout << endl;
	}
}

void BurnAroundShip(int16_t* Field, int i0, int j0, int k, int l)
{
	for(int i = i0; i <= k; i++)
	{
		for(int j = j0; j <= l; j++)
		{
			Field[10 * i + j] = -1;
		}
	}
}

void FindBurnedShip(int16_t* Field, int typeOfShip)
{
	int ShipI = -1;
        int ShipJ = -1;
	
	bool isHorisontal;

	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			if(Field[10 * i + j] == -2)
			{
				ShipI = i;
				ShipJ = j;
				break;
			}
		}

		if(ShipI != -1 && ShipJ != -1)
		{
			cout << "I'm in if()" << endl;
			break;
		}
	}

	if(typeOfShip != -1)
	{
		if(ShipI + 1 > 9)
		{
			isHorisontal = true;
		}
		else if(ShipJ + 1 > 9)
		{
			isHorisontal = false;
		}
		else
		{
			if(Field[10 * (ShipI + 1) + ShipJ] == -2)
			{
				isHorisontal = false;
			}
			else if(Field[10 * ShipI + ShipJ + 1] == -2)
			{
				isHorisontal = true;
			}
		}
	}
	else
	{
		isHorisontal = true;
	}

	if(isHorisontal)
	{
		if(ShipI - 1 < 0)	//Верх
		{
			if(ShipJ - 1 < 0)		//Верхний левый угол
			{
				BurnAroundShip(Field, ShipI, ShipJ, ShipI + 1, ShipJ + typeOfShip);
			}
			else if(ShipJ + typeOfShip > 9)	//Верхний правый угол
			{
				BurnAroundShip(Field, ShipI, ShipJ - 1, ShipI + 1, ShipJ + typeOfShip - 1);
			}
			else		//Просто верх
			{
				BurnAroundShip(Field, ShipI, ShipJ - 1, ShipI + 1, ShipJ + typeOfShip);
			}
		}
		else if(ShipI + 1 > 9)	//Низ
		{
			if(ShipJ - 1 < 0)	//Нижний левый угол
			{	
				BurnAroundShip(Field, ShipI - 1, ShipJ, ShipI, ShipJ + typeOfShip);
			}
			else if(ShipJ + typeOfShip > 9)	//Нижний правый угол
			{
				BurnAroundShip(Field, ShipI - 1, ShipJ - 1, ShipI, ShipJ + typeOfShip - 1);
			}
			else		//Просто низ
			{
				BurnAroundShip(Field, ShipI - 1, ShipJ - 1, ShipI, ShipJ + typeOfShip);
			}
		}
		else if(ShipJ - 1 < 0)	//Левый бок
		{
			BurnAroundShip(Field, ShipI - 1, ShipJ, ShipI + 1, ShipJ + typeOfShip);
		}
		else if(ShipJ + typeOfShip > 9)	//Правый бок
		{
			BurnAroundShip(Field, ShipI - 1, ShipJ - 1, ShipI + 1, ShipJ + typeOfShip - 1);
		}
		else	//Не по краям
		{
			BurnAroundShip(Field, ShipI - 1, ShipJ - 1, ShipI + 1, ShipJ + typeOfShip);
		}
	}
	else
	{
		if(ShipI - 1 < 0)	//Верх
		{
			if(ShipJ - 1 < 0)	//Верхний левый
			{
				BurnAroundShip(Field, ShipI, ShipJ, ShipI + typeOfShip, ShipJ + 1);
			}
			else if(ShipJ + 1 > 9)	//Верхний правый
			{
				BurnAroundShip(Field, ShipI, ShipJ - 1, ShipI + typeOfShip, ShipJ);
			}
			else		//Просто верх
			{
				BurnAroundShip(Field, ShipI, ShipJ - 1, ShipI + typeOfShip, ShipJ + 1);
			}
		}
		else if(ShipI + typeOfShip > 9)	//Низ
		{
                        if(ShipJ - 1 < 0)    //Нижний левый
                        {
                                BurnAroundShip(Field, ShipI - 1, ShipJ, ShipI + typeOfShip - 1, ShipJ + 1);
                        }
                        else if(ShipJ + 1 > 9)       //Нижний правый
                        {
                                BurnAroundShip(Field, ShipI - 1, ShipJ - 1, ShipI + typeOfShip - 1, ShipJ);
                        }
                        else			//Просто низ
                        {
                                BurnAroundShip(Field, ShipI - 1, ShipJ - 1, ShipI + typeOfShip - 1, ShipJ + 1);
                        }

		}
		else if(ShipJ - 1 < 0)	//Левый бок
		{
			BurnAroundShip(Field, ShipI - 1, ShipJ, ShipI + typeOfShip, ShipJ + 1);
		}
		else if(ShipJ + 1 > 9)	//Правый бок
		{
			BurnAroundShip(Field, ShipI - 1, ShipJ - 1, ShipI + typeOfShip, ShipJ);
		}
		else	//Не по краям
		{
			BurnAroundShip(Field, ShipI - 1, ShipJ - 1, ShipI + typeOfShip, ShipJ + 1);
		}
	}
}

void* DataFromFirstClient(void* NullData)
{
	stateOfFirstPlayer = PlacingShips;
	recv(FirstPlayer, &FieldOfFP, 100 * sizeof(int16_t), MSG_NOSIGNAL);
	stateOfFirstPlayer = Ready;

	showFP();

	cout << "In First Thread: " << stateOfFirstPlayer << " " << stateOfSecondPlayer << "\n";

	if(stateOfSecondPlayer == PlacingShips)
	{
		stateOfFirstPlayer = WaitingOfReadyPlayer;
		//send(*firstPlayer, &stateOfFirstPlayer, sizeof(stateOfFirstPlayer), MSG_NOSIGNAL);
		while(stateOfSecondPlayer != Ready);
	}

	stateOfFirstPlayer = Turn;
	//send(*firstPlayer, &stateOfFirstPlayer, sizeof(stateOfFirstPlayer), MSG_NOSIGNAL);

	//Информация о кораблях противника:
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
			if((FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] != -1) && (FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] != -2))
			{
                                Message MsgForSP;
                                Message MsgForFP;

                                MsgForFP.type = static_cast<int16_t>(result_of_shot);
				MsgForFP.PosX = InfoFromClient->PosX;
				MsgForFP.PosY = InfoFromClient->PosY;

                                MsgForSP.type = static_cast<int16_t>(enemy_shot);
                                MsgForSP.PosX = InfoFromClient->PosX;
                                MsgForSP.PosY = InfoFromClient->PosY;


				if(FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] != 0)
				{
					cout << "First player bung in  " << FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] << endl;
					int16_t typeOfShip = FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX];

					if(typeOfShip == 1)
					{
						--OneShip;
						FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] = -2;
						MsgForFP.Result = static_cast<int16_t>(hit);
						MsgForSP.Result = static_cast<int16_t>(hit);
					}
					else if(typeOfShip == 2)
					{
						++TwoShip[1];
						FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] = -2;

						if(TwoShip[1] == 2)
						{
							--TwoShip[0];
							TwoShip[1] = 0;

							FindBurnedShip(FieldOfSP, typeOfShip);
						}
						else
						{
							MsgForSP.Result = static_cast<int16_t>(hit);
							MsgForFP.Result = static_cast<int16_t>(hit);
						}
					}
					else if(typeOfShip == 3)
					{
						++ThreeShip[1];
						FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] = -2;

						if(ThreeShip[1] == 3)
						{
							--ThreeShip[0];
							ThreeShip[1] = 0;
						}
						else
						{
							MsgForSP.Result = static_cast<int16_t>(hit);
							MsgForFP.Result = static_cast<int16_t>(hit);
						}
					}
					else if(typeOfShip == 4)
					{
						++FourShip[1];
						FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] = -2;

						if(FourShip[1] == 4)
						{
							--FourShip[0];
							FourShip[1] = 0;
						}
						else
						{
							MsgForSP.Result = static_cast<int16_t>(hit);
							MsgForFP.Result = static_cast<int16_t>(hit);
						}
					}
				}
				else
				{
					FieldOfSP[10 * InfoFromClient->PosY + InfoFromClient->PosX] = -1;

					MsgForSP.Result = static_cast<int16_t>(not_hit);
					MsgForFP.Result = static_cast<int16_t>(not_hit);
					
					cout << "First Player not bung!\n";
					
					stateOfFirstPlayer = WaitingOfTurn;
					stateOfSecondPlayer = Turn;
				}

				send(FirstPlayer, &MsgForFP, sizeof(MsgForFP), MSG_NOSIGNAL);
				send(SecondPlayer, &MsgForSP, sizeof(MsgForSP), MSG_NOSIGNAL);

				/*cout << "FOne: " << OneShip << ", Palub: " << OneShip << endl;;
                                cout << "FTwo: " << TwoShip[0] << ", Palub: " << TwoShip[1] << endl;
                                cout << "FThree: " << ThreeShip[0] << ", Palub: " << ThreeShip[1] << endl;
                                cout << "FFour: " << FourShip[0] << ", Palub: " << FourShip[1] << endl;*/

				showSP();

				cout << endl;
				cout << endl;
			}
			else
                	{
                        	cout << "FP: " << InfoFromClient->PosX << " " << InfoFromClient->PosY << "- here burned!" << endl;
                	}

		}

		delete InfoFromClient;
	}

	pthread_exit(0);
}

void* DataFromSecondClient(void* NullData)
{
	stateOfSecondPlayer = PlacingShips;
	recv(SecondPlayer, &FieldOfSP, 100 * sizeof(int16_t), MSG_NOSIGNAL);
	stateOfSecondPlayer = Ready;

	showSP();

	cout << "In Second Thread: " << stateOfFirstPlayer << " " << stateOfSecondPlayer << "\n";

	if(stateOfFirstPlayer == PlacingShips)
	{
		stateOfSecondPlayer = WaitingOfReadyPlayer;
		//send(*secondPlayer, &stateOfSecondPlayer, sizeof(stateOfSecondPlayer), MSG_NOSIGNAL);

		while(stateOfFirstPlayer != Ready);
	}
	
	stateOfSecondPlayer = WaitingOfTurn;
	//send(*secondPlayer, &stateOfSecondPlayer, sizeof(stateOfSecondPlayer), MSG_NOSIGNAL);

	//Информация о кораблях противника:
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
			if((FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] != -1) && (FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] != -2))
			{
				Message MsgForFP;
				Message MsgForSP;

				MsgForFP.type = static_cast<int16_t>(enemy_shot);
				MsgForFP.PosX = InfoFromClient->PosX;
				MsgForFP.PosY = InfoFromClient->PosY;

				MsgForSP.type = static_cast<int16_t>(result_of_shot);
				MsgForSP.PosX = InfoFromClient->PosX;
				MsgForSP.PosY = InfoFromClient->PosY;

				if(FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] != 0)
				{
					cout << "Second player bung in " << FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] << endl;
					int16_t typeOfShip = FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX];

					if(typeOfShip == 1)
					{
						--OneShip;
						FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] = -2;

						MsgForFP.Result = static_cast<int16_t>(hit);
						MsgForSP.Result = static_cast<int16_t>(hit);
					}
					else if(typeOfShip == 2)
					{
						++TwoShip[1];
						FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] = -2;

						if(TwoShip[1] == 2)
						{
							--TwoShip[0];
							TwoShip[1] = 0;
						}
						else
						{
							MsgForFP.Result = static_cast<int16_t>(hit);
							MsgForSP.Result = static_cast<int16_t>(hit);
						}
					}
					else if(typeOfShip == 3)
					{
						++ThreeShip[1];
						FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] = -2;

						if(ThreeShip[1] == 3)
						{
							--ThreeShip[0];
							ThreeShip[1] = 0;
						}
						else
						{
							MsgForFP.Result = static_cast<int16_t>(hit);
							MsgForSP.Result = static_cast<int16_t>(hit);
						}
					}
					else if(typeOfShip == 4)
					{
						++FourShip[1];
						FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] = -2;

						if(FourShip[1] == 4)
						{
							--FourShip[0];
							FourShip[1] = 0;
						}
						else
						{
							MsgForFP.Result = static_cast<int16_t>(hit);
							MsgForSP.Result = static_cast<int16_t>(hit);
						}
					}
				}
				else
				{
					FieldOfFP[10 * InfoFromClient->PosY + InfoFromClient->PosX] = -1;

					MsgForSP.Result = static_cast<int16_t>(not_hit);
					MsgForFP.Result = static_cast<int16_t>(not_hit);

					cout << "Second player not bung!\n";
					
					stateOfSecondPlayer = WaitingOfTurn;
					stateOfFirstPlayer = Turn;
				}

				send(FirstPlayer, &MsgForFP, sizeof(MsgForFP), MSG_NOSIGNAL);
				send(SecondPlayer, &MsgForSP, sizeof(MsgForSP), MSG_NOSIGNAL);
				/*cout << "SOne: " << OneShip << ", Palub: " << OneShip << endl;
				cout << "STwo: " << TwoShip[0] << ", Palub: " << TwoShip[1] << endl;
				cout << "SThree: " << ThreeShip[0] << ", Palub: " << ThreeShip[1] << endl;
				cout << "SFour: " << FourShip[0] << ", Palub: " << FourShip[1] << endl;*/
				
				showFP();
				
				cout << endl;
				cout << endl;
			}
			else
                	{
                       		cout << "SP: " << InfoFromClient->PosX << " " << InfoFromClient->PosY << "- here burned!" << endl;
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

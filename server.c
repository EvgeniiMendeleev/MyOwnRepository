#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdint.h>

//-------------Состояния--------------------(Надо будет уточнить по поводу протокола)
#define STOP -2
#define WaitingOfConnection -1
#define WaitingOfTurn 0
#define Turn 1
//------------------------------------------

//---------Пакет информации клиента---------
struct Position{

	float x, y;
};
//------------------------------------------

int main()
{
	//-----------Создание слушащего сокета---------------------

	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in MasSocket;
	MasSocket.sin_family = AF_INET;
	MasSocket.sin_port = htons(25567);
	MasSocket.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(MasterSocket,(struct sockaddr*) (&MasSocket), sizeof(MasSocket));
	
	listen(MasterSocket, 1);
	//---------------------------------------------------------

	//-----------Ожидание соединения клиентов------------------

	int FirstPlayer = accept(MasterSocket, 0, 0);
	
	if(FirstPlayer > 0){
		
		printf("First player connected\n");
		int_least8_t state = WaitingOfConnection;
		send(FirstPlayer, &state, sizeof(state), MSG_NOSIGNAL);
	}
	else{
	
		printf("Connection had failed\n");
		return 0;
	}

	int SecondPlayer = accept(MasterSocket, 0, 0);

	if(SecondPlayer > 0){
	
		printf("Second player connected!\n");
		uint_least8_t stateFirstPlayer = Turn;
		uint_least8_t stateSecondPlayer = WaitingOfTurn;
		send(FirstPlayer, &stateFirstPlayer, sizeof(stateFirstPlayer), MSG_NOSIGNAL);
		send(SecondPlayer, &stateSecondPlayer, sizeof(stateSecondPlayer), MSG_NOSIGNAL);
	}
	else {
	
		printf("Connection had failed\n");
		return 0;
	}

	//---------------------------------------------------------
	
	//---------------------Процесс обработки данных---------------
	printf("The game have begun!\n");

	sleep(5);

	int_least8_t nextStateFirstPlayer = WaitingOfTurn, nextStateSecondPlayer = Turn;	//Следующее состояние клиента

	while(1){

		struct Position Buffer;

		recv(FirstPlayer, &Buffer, sizeof(Buffer), MSG_NOSIGNAL);	
		
		//-----------Проверка данных-------------------
		//---------------------------------------------

		send(SecondPlayer, &Buffer, sizeof(Buffer), MSG_NOSIGNAL);

		//----Обмен состояниями: теперь первый игрок ждёт, а второй ходит----

		send(FirstPlayer, &nextStateFirstPlayer, sizeof(nextStateFirstPlayer), MSG_NOSIGNAL);
		send(SecondPlayer, &nextStateSecondPlayer, sizeof(nextStateSecondPlayer), MSG_NOSIGNAL);
		
		int t = FirstPlayer;
		FirstPlayer = SecondPlayer;
		SecondPlayer = t;

		//-----------------------------------------
	}
	//------------------------------------------------------------

	return 0;
}

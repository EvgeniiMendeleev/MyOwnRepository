#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

//----------Состояния клиента-------
#define Stop -2
#define WaitingOfConnection -1
#define WaitingOfTurn 0
#define Turn 1
//----------------------------------

//----------Координаты корабля---------
struct PositionOfShip{

	float x, y;
};
//-------------------------------------

int main(){

	//----------------------Создание клиентского сокета и подключение клиента------------------
	int_least8_t state;	//Текущее состояние клиента

	int SendSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in SSocket;

	SSocket.sin_family = AF_INET;
	SSocket.sin_port = htons(25567);
	inet_pton(AF_INET, "192.168.0.103", &(SSocket.sin_addr));

	if(connect(SendSocket, (struct sockaddr*) (&SSocket), sizeof(SSocket)) == -1){

		printf("error\n");
		return 0;
	}
	//-----------------------------------------------------------------------------------------


	//---------------Ожидание второго игрока------------------
	
	//Читаем состояние клиента.
	//Если мы будем ожидать подключение, то значит это был первый игрок,
	//иначе это был второй игрок, и мы идём дальше, давая второму игроку
	//состояние ОЖИДАНИЕ ХОДА ПРОТИВНИКА, отправляя с сервера.
	recv(SendSocket, &state, sizeof(state), MSG_NOSIGNAL);

	if(state == WaitingOfConnection){

		printf("Waiting of player\n");
		recv(SendSocket, &state, sizeof(state), MSG_NOSIGNAL);
	}
	//--------------------------------------------------------
	
	//------------------------Процесс игры-------------------------
	printf("The game have begun\n");

	sleep(5);

	while(1){

		struct PositionOfShip Buffer;

		system("clear");

		if(state == Turn){

			printf("\nEnter coordinates of cell: ");
			scanf("%f %f", &Buffer.x, &Buffer.y);
			send(SendSocket, &Buffer, sizeof(Buffer), MSG_NOSIGNAL);
			
			//Чтение следующего состояния клиента.
			recv(SendSocket, &state, sizeof(state), MSG_NOSIGNAL);
		}
		else if(state == WaitingOfTurn){
			
			printf("Waiting\n");
			recv(SendSocket, &Buffer, sizeof(Buffer), MSG_NOSIGNAL);

			//Чтение следующего состояния клиента.
			recv(SendSocket, &state, sizeof(state), MSG_NOSIGNAL);
			printf("Shot in coordinates: %f %f", Buffer.x, Buffer.y);
		}
	}
	//-------------------------------------------------------------

	shutdown(SendSocket, SHUT_RDWR);

	return 0;
}

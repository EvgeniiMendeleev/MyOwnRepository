#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct Position{

	float x, y;
};

int main(){

	while(1){

		struct Position Buffer;
		printf("Enter coordinates of cell");
		scanf(" %f %f", &Buffer.x, &Buffer.y);
		int SendSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		struct sockaddr_in SSocket;

		SSocket.sin_family = AF_INET;
		SSocket.sin_port = htons(25567);
		SSocket.sin_addr.s_addr = htonl(INADDR_ANY);

		connect(SendSocket, (struct sockaddr*) (&SSocket), sizeof(SSocket));
		send(SendSocket, &Buffer, sizeof(Buffer), MSG_NOSIGNAL);	
		shutdown(SendSocket, SHUT_RDWR);
		//close(SendSocket);
	}

	return 0;
}

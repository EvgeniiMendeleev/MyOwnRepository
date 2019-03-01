#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct Position{

	float x, y;
};

int main()
{
	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in MasSocket;
	MasSocket.sin_family = AF_INET;
	MasSocket.sin_port = htons(25567);
	MasSocket.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(MasterSocket,(struct sockaddr*) (&MasSocket), sizeof(MasSocket));
	
	listen(MasterSocket, 1);
	
	while(1)
	{
		int FriendSocket = accept(MasterSocket, 0, 0);
		//EnemySocket = accept(MasterSocket, 0, 0);

		struct Position Buffer;

		recv(FriendSocket, &Buffer, sizeof(Buffer), MSG_NOSIGNAL);
		printf("Enemy position: %f %f", Buffer.x, Buffer.y);
	
		printf("Yes\n");
		//send(EnemySocket, PositionOfEnemy, MSG_NOSIGNAL);
		
	}

	return 0;
}

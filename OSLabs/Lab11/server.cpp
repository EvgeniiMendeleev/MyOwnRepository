#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <stdint.h>
#include <clocale>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	if(argc < 2)
	{
		cout << "Недостаточно параметров" << endl;
		return -1;
	}

	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in MasterAddr;
	MasterAddr.sin_family = AF_INET;
	MasterAddr.sin_port = htons(atoi(argv[1]));
	MasterAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(MasterSocket, (struct sockaddr*)(&MasterAddr), sizeof(MasterAddr)) < 0)
	{
		perror("Порт занят");
		MasterAddr.sin_port = htons(0);
		if(bind(MasterSocket, (struct sockaddr*) (&MasterAddr), sizeof(MasterAddr)) == 0)
		{
			cout << "Теперь порт свободен " << endl;
		}
	}

	cout << "Порт: " << ntohs(MasterAddr.sin_port) << endl;

	listen(MasterSocket, 1);

	int ClientSocket = accept(MasterSocket, 0, 0);
	
	int16_t N;
	recv(ClientSocket, &N, sizeof(N), MSG_NOSIGNAL);
	cout << "N = " << N << endl;

	int16_t* Array = new int16_t[N];

	for(int i = 0; i < N; i++)
	{
		recv(ClientSocket, &Array[i], sizeof(int16_t), MSG_NOSIGNAL);
	}

	cout << "============================" << endl;
	for(int i = 0; i < N; i++)
	{
		cout << Array[i] << " ";
	}
	cout << endl;
	cout << "============================" << endl;

	for(int i = 0; i < N; i++)
	{
		for(int j = i + 1; j < N; j++)
		{
			if(Array[i] < Array[j])
			{
				int16_t t = Array[i];
				Array[i] = Array[j];
				Array[j] = t;
			}
		}
	}

	for(int i = 0; i < N; i++)
	{
		send(ClientSocket, &Array[i], sizeof(int16_t), MSG_NOSIGNAL);
	}

	cout << endl;
	
	shutdown(ClientSocket, SHUT_RDWR);

	delete[] Array;
	return 0;
}

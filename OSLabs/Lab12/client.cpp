#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <clocale>
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

using namespace std;

int SendAll(int socket, void* buffer, int len, int flags)
{
	int8_t* Info = static_cast<int8_t*>(buffer);

	int total = 0;
	int n;

	while(total < len)
	{
		n = send(socket, Info + total, len - total, flags);
		if(n == -1) break;
		total += n;
	}

	return (n == -1 ? -1 : total);
}

int RecvAll(int socket, void* buffer, int len, int flags)
{
	int8_t* Info = static_cast<int8_t*>(buffer);

	int total = 0;
	int n;

	while(total < len)
	{
		n = recv(socket, Info + total, len - total, flags);
		if(n == -1) break;
		total += n;
	}

	return (n == -1 ? -1 : total);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	if(argc < 2)
	{
		cout << "Мало аргументов" << endl;
		return -1;
	}
	

	char port[5];
	vector<char> IP;

	int ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in ServAddr;
	ServAddr.sin_family = AF_INET;
	
	for(int i = strlen(argv[1]) - 5, j = 0; i < strlen(argv[1]); i++, j++)
	{
		port[j] = argv[1][i];
		cout << port[j];
	}
	cout << endl;

	for(int i = 0; argv[1][i] != ':'; i++)
	{
		IP.push_back(argv[1][i]);
	}

	char* Ip = new char[IP.size() + 1];

	for(int i = 0; i < IP.size(); i++)
	{
		Ip[i] = IP[i];
		cout << Ip[i];
	}
	cout << endl;

	Ip[IP.size()] = '\0';

	ServAddr.sin_port = htons(atoi(port));
	inet_pton(AF_INET, Ip, &(ServAddr.sin_addr));

	if(connect(ClientSocket, (struct sockaddr*) (&ServAddr), sizeof(ServAddr)) == 0)
	{
		cout << "Подключение прошло успешно" << endl;
	}
	else 
	{
		perror("Подключение: ");
	}

	cout << endl;

	while(1)
	{
		int16_t command = -1;
		cout << "Введите команду:" << endl;
		cout << "1.Скачать файл" << endl;
		cout << "2.Завершить работу" << endl;
		cin >> command;

		cout << "command = " << command << endl;

		SendAll(ClientSocket, &command, sizeof(int16_t), MSG_NOSIGNAL);

		if(command == 1)
		{
			uint8_t Symbol;
			vector<uint8_t> Array;
			
			cout << "Введите полное имя файла: ";
			cin >> Symbol;

			do
			{
				Array.push_back(Symbol);
			}while((Symbol = getchar()) != '\n');

			cout << endl;

			cout << "Введённый файл: ";
			for(int i = 0; i < Array.size(); i++)
			{
				cout << static_cast<char>(Array[i]);
			}

			cout << endl;

			int16_t size = Array.size();

			cout << "size = " << size << endl;

			SendAll(ClientSocket, &size, sizeof(int16_t), MSG_NOSIGNAL);

			for(int i = 0; i < size; i++)
			{
				SendAll(ClientSocket, &Array[i], sizeof(uint8_t), MSG_NOSIGNAL);
			}

			int16_t ResultOfAccess;
			RecvAll(ClientSocket, &ResultOfAccess, sizeof(int16_t), MSG_NOSIGNAL);

			if(!ResultOfAccess)
			{
				cout << "Указанного файла не существует" << endl;
			}
			else
			{
				int position = 0;

				for(int i = Array.size() - 1; Array[i] != '/'; i--)
				{
					++position;
				}

				char* NameOfFile = new char[position + 1];
				cout << "position = " << position << endl;

				for(int i = 0, j = Array.size() - position; i < position; i++, j++)
				{
					NameOfFile[i] = Array[j];
					cout << NameOfFile[i];
				}
				cout << endl;
				
				NameOfFile[position] = '\0';

				uint32_t ByteOfFile;
				cout << "Файл существует в архиве сервера" << endl;

				RecvAll(ClientSocket, &ByteOfFile, sizeof(uint32_t), MSG_NOSIGNAL);
				cout << "Число элементов в файле: " << ByteOfFile << endl;

				int NewFile = open(NameOfFile, O_CREAT | O_RDWR, 0600);
				ftruncate(NewFile, ByteOfFile);

				if(NewFile == -1)
				{
					cout << "Ошибка с созданием файла" << endl;
				}

				uint32_t* DataFromCreateFile = (uint32_t*)mmap(NULL, ByteOfFile, PROT_WRITE|PROT_READ, MAP_SHARED, NewFile, 0);

				cout << "RecvAll = " << RecvAll(ClientSocket, DataFromCreateFile, ByteOfFile, MSG_NOSIGNAL) << endl;

				munmap(DataFromCreateFile, ByteOfFile);
				close(NewFile);
				delete[] NameOfFile;
			}

			Array.clear();
		}
		else if(command == 2)
		{
			shutdown(ClientSocket, SHUT_RDWR);
			close(ClientSocket);
			return 0;
		}

	}

	return 0;
}

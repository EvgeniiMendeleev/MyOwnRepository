#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <clocale>
#include <iostream>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <stdint.h>
#include <errno.h>
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

	if(n == -1)
	{
		return -1;
	}
	else
	{
		return total;
	}
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

	if(n == -1)
	{
		perror("Не все байты получены\n");
		
		if(errno == EFAULT)
		{
			cout << "Ябадабадууууу!"<<endl;
		}

		return -1;
	}
	else
	{
		cout << "Все байты получены" << endl;
		return total;
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	if(argc < 2)
	{
		cout << "Недостаточно аргументов" << endl;
		return -1;
	}

	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in MasterAddr;
	MasterAddr.sin_family = AF_INET;
	MasterAddr.sin_port = htons(atoi(argv[1]));
	MasterAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(MasterSocket, (struct sockaddr*) (&MasterAddr), sizeof(MasterAddr)) < 0)
	{
		perror("Порт занят");
		MasterAddr.sin_port = 0;

		if(bind(MasterSocket, (struct sockaddr*)(&MasterAddr), sizeof(MasterAddr)) >= 0)
		{
			socklen_t size = sizeof(MasterAddr);
			if(getsockname(MasterSocket, (struct sockaddr*) &MasterAddr, &size) == 0)
			{
				cout << "Теперь порт свободен" << endl;
			}
		}
	}

	cout << "Порт: " << ntohs(MasterAddr.sin_port) << endl;

	listen(MasterSocket, SOMAXCONN);

	while(1)
	{
		int command;
		cout << "Введите комманду: " << endl;
		cout << "1.Принять соединение" << endl;
		cout << "2.Завершить работу" << endl;
		cin >> command;

		cout << "command = " << command << endl;
		if(command == 1)
		{
			int ClientSocket = accept(MasterSocket, 0, 0);

			if(ClientSocket >= 0)
			{
				pid_t process = fork();
	
				if(process < 0)
				{
					cout << "Ошибка с fork()" << endl;
				}
				else if(process == 0)
				{
					cout << "Клиент подключился" << endl;

					while(1)
					{
						int16_t CommandOfClient;

						RecvAll(ClientSocket, &CommandOfClient, sizeof(int16_t), MSG_NOSIGNAL);

						cout << "Команда клиента: ";

						if(CommandOfClient == 1)
						{
							cout << "Скачать файл" << endl;

							int16_t size;
							
							RecvAll(ClientSocket, &size, sizeof(int16_t), MSG_NOSIGNAL);
							
							cout << "Размер названия файла: " << size << endl;

							uint8_t* Array = new uint8_t[size];

							RecvAll(ClientSocket, Array, size * sizeof(uint8_t), MSG_NOSIGNAL);

							cout << "Название файла: ";
							for(int i = 0; i < size; i++)
							{
								cout << Array[i];
							}
							cout << endl;

							char* NameOfFile = new char[size + 1];
							NameOfFile[size] = '\0';

							for(int i = 0; i < size; i++)
							{
								NameOfFile[i] = Array[i];
								cout << NameOfFile[i];
							}
							cout << endl;

							int DownloadFile = open(NameOfFile, O_RDONLY, 0600);
							int16_t Access;

							if(DownloadFile == -1)
							{
								if(errno == ENOENT)
								{
									cout << "Указанный файл не существует" << endl;
									Access = 0;
									SendAll(ClientSocket, &Access, sizeof(int16_t), MSG_NOSIGNAL);
								}
							}
							else
							{
								Access = 1;
								SendAll(ClientSocket, &Access, sizeof(int16_t), MSG_NOSIGNAL);
								cout << "Файл есть в архиве сервера" << endl;

								struct stat InfoAboutFile;

								fstat(DownloadFile, &InfoAboutFile);
								
								uint32_t ByteOfFile = InfoAboutFile.st_size;
								SendAll(ClientSocket, &ByteOfFile, sizeof(uint32_t), MSG_NOSIGNAL);

								uint32_t* DataFromFile = (uint32_t*)mmap(NULL, InfoAboutFile.st_size, PROT_READ, MAP_SHARED, DownloadFile, 0);

								SendAll(ClientSocket, DataFromFile, ByteOfFile, MSG_NOSIGNAL);
							}

							close(DownloadFile);
							delete[] NameOfFile;
							delete[] Array;

						}
						else if(CommandOfClient == 2)
						{
							cout << "Завершить работу" << endl;

							shutdown(ClientSocket, SHUT_RDWR);
							close(ClientSocket);
							return 0;
						}
					}
				}
			}
			else
			{
				cout << "Неудачное соединение" << endl;
			}
		}
		else if(command == 2)
		{
			shutdown(MasterSocket, SHUT_RDWR);
			close(MasterSocket);
			
			return 0;
		}
	}
}

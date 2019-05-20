#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <stdint.h>
#include <clocale>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>
#include <chrono>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
{
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	char port[5];

	if(argc < 2)
	{
		cout << "Мало аргументов" << endl;
		return 0;
	}

	chrono::time_point<chrono::system_clock> start, end;

	int ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in ServAddr;
	ServAddr.sin_family = AF_INET;

       	for(int i = strlen(argv[1]) - 5, j = 0; i < strlen(argv[1]); i++, j++)
       	{
       	        port[j] = argv[1][i];
       	}

        ServAddr.sin_port = htons(atoi(port));

	inet_pton(AF_INET, argv[1], &(ServAddr.sin_addr));

	if(connect(ClientSocket, (struct sockaddr*) (&ServAddr), sizeof(ServAddr)) == 0)
	{
		cout << "Соединение успешно завершено" << endl;
	}

        int16_t N;
        cout << "Введите число цифр: ";
        cin >> N;

        send(ClientSocket, &N, sizeof(N), MSG_NOSIGNAL);

	int16_t MaxValue, MinValue;

        cout << "\nВведите максимальное, а затем минимальное число" << endl;
        cin >> MaxValue >> MinValue;

	int16_t* Array = new int16_t[N];

	cout << "\n===================================" << endl;
	for(int i = 0; i < N; i++)
	{
                Array[i] = rand() % MaxValue + MinValue;
                
		cout << Array[i] << " ";

		send(ClientSocket, &Array[i], sizeof(int16_t), MSG_NOSIGNAL);
        }
	cout << "\n===================================" << endl;

	start = chrono::system_clock::now();
	for(int i = 0; i < N; i++)
	{
		recv(ClientSocket, &Array[i], sizeof(int16_t), MSG_NOSIGNAL);
	}
	end = chrono::system_clock::now();

	cout << "\nРезультат сортировки: " << endl;
	for(int i = 0; i < N; i++)
	{
		cout << Array[i] << " ";
	}
	cout << endl;

	int dt = chrono::duration_cast<chrono::microseconds>(end - start).count();

	cout << "Время ожидания от сервера: " << dt << " мс\n";

	shutdown(ClientSocket, SHUT_RDWR);

	delete[] Array;
	return 0;
}

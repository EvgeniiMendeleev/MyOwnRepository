#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[])
{
	FILE* ReadFile = fopen(argv[1], "r");

	if(ReadFile == NULL)
	{
		perror("cannot open file");
		return -1;
	}

	int N = atoi(argv[2]) / sizeof(char);
	char* data = new char[N];

	size_t t;
	t = fread(data, sizeof(char), atoi(argv[2]), ReadFile);
	
	cout << "count of read bytes = " << t << endl;

	for(int i = 0; i < t; i++)
	{
		cout << data[i] << ": " << static_cast<int>(data[i]) << endl;
	}

	delete data;
	return 0;
}

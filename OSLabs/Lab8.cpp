#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		cout << "Malo malo malo! parametrs\n";
		return -1;
	}

	int WriteFile = open(argv[2], O_RDWR | O_CREAT, 0600);
	int ReadFile = open(argv[1], O_RDONLY, 0600);
	struct stat InfoAboutFile;

	fstat(ReadFile, &InfoAboutFile);
	ftruncate(WriteFile, InfoAboutFile.st_size);

	int N = InfoAboutFile.st_size / sizeof(char);
	char* DataFromFile = new char[N];

	read(ReadFile, DataFromFile, InfoAboutFile.st_size);

	char* DataOfWF = (char*)mmap(NULL, InfoAboutFile.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, WriteFile, 0);

	for(int i = 0; i < N; i++)
	{
		DataOfWF[i] = DataFromFile[i];
	}

	return 0;
}

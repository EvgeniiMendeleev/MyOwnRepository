#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>

unsigned SIGINTcount = 0;
unsigned SIGUSR1count = 0;
unsigned SIGUSR2count = 0;

void handl_int(int nsig)
{
  	SIGINTcount++;
}
void handl_usr1(int nsig)
{
	SIGUSR1count++;
}
void handl_usr2(int nsig)
{
	SIGUSR2count++;
}
int main()
{
	signal(SIGTERM, SIG_IGN);
	signal(SIGUSR1,handl_usr1);
	signal(SIGUSR2,handl_usr2);
    signal(SIGINT, handl_int);
    for(size_t i = 0;;i++)
    {
    	if(i % 2 == 0)
    	{
    		raise(SIGUSR1);			
    	}
    	if(i % 5 == 0)
    	{
    		raise(SIGUSR2);
    	}
    	if(SIGINTcount < 5)
    	{
    		system("date");
    		std::cout<<"SIGINT :"<<SIGINTcount<<" SIGUSR1: "<<SIGUSR1count<<" SIGUSR2: "<<SIGUSR2count<<std::endl;
    	    sleep(1);
    	}
    	else
    	{
    		std::cout<<"SIGINT :"<<SIGINTcount<<" SIGUSR1: "<<SIGUSR1count<<" SIGUSR2: "<<SIGUSR2count<<std::endl;
    		return 0;
    	}
    }
    return 0;
}

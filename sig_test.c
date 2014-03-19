#include "allhead.h"

void ouch(int sig)
{
	printf("I got a signal %d\n",sig);
	(void) signal(SIGINT, SIG_DFL);
}

int main()
{
	(void) signal(SIGINT,ouch);
	while(1)
	{
		puts("help!");
		sleep(1);
	}
	return EXIT_SUCCESS;
}

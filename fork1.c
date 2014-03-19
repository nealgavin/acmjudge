#include "allhead.h"

int main()
{
	pid_t pid;
	char *message;
	int n;
	int exit_code;
	puts("fork start!");
	pid = fork();
	switch(pid)
	{
		case -1:
			perror("fork error!");
			break;
		case 0:
			message = "This is the child";
			n = 7;
			exit_code = 37;
			break;
		default:
			message = "This is parent";
			n = 3;
			exit_code = 0;
			break;
	}

	for(;n>0;--n)
	{
		puts(message);
		sleep(1);
	}
	if (pid != 0)
	{
		int stat_val;
		pid_t child_pid;
		child_pid = wait(&stat_val);
		printf("Child has finished :PID = %d\n",child_pid);
		if(WIFEXITED(stat_val))
			printf("Child exited with code %d\n",WEXITSTATUS(stat_val));
		else
			printf("Child term");
	}
	return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int status;
	pid_t pid;

	pid = fork();

	if(pid==0)
	{
		return 3;
	}
	else
	{
		printf("Child PID: %d\n", pid);
		
		pid = fork();

		if(pid==0)
		{
			exit(7);
		}
		else
		{
			printf("Child PID : %d\n", pid);

			wait(&status);

			if(WIFEXITED(status))
			{
				printf("Child sned one: %d\n", WEXITSTATUS(status));
			}

			wait(&status);

			if(WIFEXITED(status))
			{
				printf("Child sned two: %d\n", WEXITSTATUS(status));
			}

			sleep(30);
		}
	}
	return 0;
}

#include <stdio.h>
#include <unistd.h>

int main(int arc, char* argv[])
{
	pid_t pid;

	pid = fork();

	if(pid==0)
	{
		printf("Hi, I am a child process.\n");
	}
	else
	{
		printf("Child Process ID : %d\n", pid);
		sleep(30);
	}

	if(pid==0)
	{
		puts("End Chile Process.");
	}
	else
	{
		puts("End parent process.");
	}

	return 0;
}


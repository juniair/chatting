#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define BUF_SIZE 30

void error_handling(char *message);
void read_chileproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;

	pid_t pid;
	struct sigaction act;
	socklen_t addr_size;
	int str_len, state;
	char buf[BUF_SIZE];

	if(argc != 2)
	{
		printf("Usage : %s, <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler = read_chileproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);
	

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error");
	}
	
	if(listen(serv_sock, 5) == -1)
	{
		error_handling("listen() error");
	}

	while(1)
	{
		addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &addr_size);
		if(clnt_sock == -1)
		{
			continue;
		}
		else
		{
			puts("nuw client connected...");
		}

		pid = fork();

		switch(pid)
		{
			case -1:
				close(clnt_sock);
				continue;
			case 0:
				close(serv_sock);
				while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
				{
					sleep(5);
					write(clnt_sock, buf, BUF_SIZE);
				}
				close(clnt_sock);
				puts("client disconnected...");
				exit(0);
			default:
				close(clnt_sock);

		}

	}

	close(serv_sock);
	exit(0);
}

void read_chileproc(int sig)
{
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("remove proc id: %d\n", pid);
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

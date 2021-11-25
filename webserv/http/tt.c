#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	char *argv[] = {"./cgi_tester", "./PATH_INFO", NULL};
	char *envp[] = {"REQUEST_METHOD=GET", "SERVER_PROTOCOL=HTTP/1.1", "PATH_INFO=/i", NULL};

	pid_t pid;

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);

	write(fdIn, "qwer", 4);
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();
	if (pid == 0)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve("./cgi_tester", argv, envp);
	}
	else
	{
		char buf[1024] = {0, };
		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		int ret = read(fdOut, buf, 1024);
		buf[ret] = 0;
		printf("%s\n", buf);
	}
	return 0;
}
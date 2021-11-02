#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unsitd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 10124;

int main(int ag, char **av)
{
	int serv_sock;
	int clnt_sock;
	char msg[BUF_SIZE];
	int str_len;
	int i;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	socklen_t clnt_addr_sz;

	if (ag != 2)
	{
		std::cout << "Usage :" << av[0]  << std::endl;
		exit(1);

	}
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock

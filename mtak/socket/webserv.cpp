#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void *request_handler(void *ag)
{
	
void error_handler(std::string msg)
{
	std::cout << "\033[35m" << msg << "\033[37m"<< std::endl;
	exit(1);
}

void print_handler(std::string msg)
{
	std::cout << "\033[35m" << msg << "\033[37m"<< std::endl;
}

int main(int ag, char **av)
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	char buf[BUF_SIZE];
	pthread_t t_id;

	if (ag != 2)
		error_handler("argument error");
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handler("socket error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(av[1]));
	if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		error_handler("bind error");
	if (listen(serv_sock, 20) == -1)
		error_handler("listen error");
	while (true)
	{
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
		print_handler("Connection Request :" + inet_ntoa(clnt_addr.sin_addr) + ntohs(clnt_addr.sin_port));
		pthread_create(&t_id, NULL, request_handler, &clnt_sock);
		pthread_detach(t_id);
	}
	close(serv_sock);
	return 0;
}


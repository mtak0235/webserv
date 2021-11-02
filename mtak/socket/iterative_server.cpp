#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handler(std::string msg)
{
	std::cout << msg << std::endl;
	exit(1);
}

int main(int ag, char **av)
{
	int server_sock;
	int client_sock;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	socklen_t client_addr_size;

	if (ag != 2)
	{
		std::cout << "Usage :" << av[0] << std::endl;
		exit(1);

	}
	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (server_sock == -1)
		error_handler("socket error");
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(av[1]));
	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		error_handler("bind error");
	if (listen(server_sock, 5) == -1)
		error_handler("listen error");
	client_addr_size = sizeof(client_addr);
	for (int i = 0; i < 5; i++)
	{
		client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
		if (client_sock == -1)
			error_handler("accept error");
		else
			std::cout << "client connected :" << i + 1 << std::endl;
		int str_len;
		char msg[BUF_SIZE];
		while ((str_len = read(client_sock, msg, BUF_SIZE)) != 0)
			write(client_sock, msg, str_len);
		close(client_sock);
	}
	close(server_sock);
	return 0;
}	

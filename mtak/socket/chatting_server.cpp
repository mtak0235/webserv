#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>

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
	struct sockaddr_in client_addr; //accept()
	socklen_t client_addr_size;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);//ipv4, tcp
	if (server_sock == -1)
		error_handler("socket error");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;//ipv4
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//ip address
	server_addr.sin_port = htons(atoi(av[1]));

	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		error_handler("bind error");

	if (listen(server_sock, 5) == -1)
		error_handler("listen error");
	
	client_addr_size = sizeof(client_addr);
	client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
	if (client_sock == -1)
		error_handler("accept error");
	
	char msg[] = "ehi This is server\n";
	write(client_sock, msg, sizeof(msg));

	close(client_sock);
	close(server_sock);

	return 0;
}

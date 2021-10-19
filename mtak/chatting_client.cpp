#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handler(std::string msg)
{
	std::cout << msg << std::endl;
	exit(1);
}

int main(int ag, char **av)
{
	int client_sock;
	struct sockaddr_in server_addr;
	char msg[1024] = {0x00, };

	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sock == -1)
		error_handler("socekt error");
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(av[1]);
	server_addr.sin_port = htons(atoi(av[2]));

	if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		error_handler("connect error");
	
	if (read(client_sock, msg, sizeof(msg) - 1) == -1)
		error_handler("read error");
	std::cout << "MSG from server:" << msg << std::endl;

	close(client_sock);
	return 0;
}
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024

void error_handler(std::string msg)
{
	std::cout << msg << std::endl;
	exit(1);
}

int main(int ag, char **av)
{
	int client_sock;
	struct sockaddr_in server_addr;
	char msg[BUFSIZE] = {0x00, };
	int received_len;

	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sock == -1)
		error_handler("socket error");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(av[1]);
	server_addr.sin_port = htons(atoi(av[2]));

	if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		error_handler("\033[35mConnection Error\033[37m");
	else
	{
		struct sockaddr_in client_addr;
		char client_addr_str[16];
		socklen_t client_addr_len = sizeof(client_addr);
		getsockname(client_sock, (struct sockaddr *)&client_addr, &client_addr_len);
		strncpy(client_addr_str, inet_ntoa(client_addr.sin_addr), 16);
		std::cout << "\033[35mConnected!!\033[37m" << std::endl;
		std::cout << "socket : \"My ip is\n" << client_addr_str << "\"\n" << std::endl;
	}

	while (true)
	{
		std::cout << "Msg to send : ";
		std::cin >> msg;
		if (strcmp(msg, "exit") == 0)
		{
			send(client_sock, msg, (int)strlen(msg), 0);
			break; 
		}
		send(client_sock, msg, (int)strlen(msg), 0);

		received_len = recv(client_sock, msg, sizeof(msg) - 1, 0);
		msg[received_len] = '\0';
		if (strcmp(msg, "exit") == 0)
		{
			std::cout << "\033[35mClose connection\033[37m" << std::endl;
			break;
		}
		std::cout << "Received msg : " << msg << std::endl;
	}
	close(client_sock);
}

	
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
	char msg[BUFSIZE];
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int server_sock;
	int client_sock;
	socklen_t client_addr_size;
	int received_len;

	if (ag != 2) {
		std::cout << "Please, Insert Port Number";
		exit(1);
	}
	
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == -1)
		error_handler("socket error");
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(av[1]));

	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		error_handler("bind error");
	if (listen(server_sock, 5) == -1)
		error_handler("listen error");

	client_addr_size = sizeof(client_addr);
	client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
	if (client_sock == -1)
		error_handler("accept error");
	else
		std::cout << "\033[35mConnected!!\033[37m" << std::endl;
	close(server_sock);
	
	while (true)
	{
		std::cout << "\033[35mMSG Receiving\033[37m" << std::endl;
		received_len = recv(client_sock, msg, sizeof(msg) - 1, 0);
		msg[received_len] = '\0';
		
		if (!strcmp(msg, "exit"))
		{
			std::cout << "\033[35mClose connection\033[37m" << std::endl;
			break; 
		}
		std::cout << "Received msg : " << msg << std::endl;
		std::cout << "Msg to send : ";
		std::cin >> msg;
		if (strcmp(msg, "exit") == 0)
		{
			send(client_sock, msg, (int)strlen(msg), 0);
			break;
		}
		send(client_sock, msg, (int)strlen(msg), 0);
	}
	close(client_sock);
	std::cout << "\033[35mClose connection\033[37m" << std::endl;
	return 0;
}

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
    int client_sock;
    struct sockaddr_in server_addr;
    char msg[BUF_SIZE];
    int str_len;

    if (ag != 3)
	{
		std::cout << "Usage :" << av[0] << std::endl;
		exit(1);
	}
    client_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (client_sock == -1)
		error_handler("socket error");
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(av[1]);
	server_addr.sin_port = htons(atoi(av[2]));
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        error_handler("connect error");
    else
        std::cout << "Connected..." << std::endl;
    while (true)
    {
        std::cout << "Msg to send (\"exit to quit\"): ";
		std::cin >> msg;
        if (strcmp(msg, "exit") == 0)
		{
			std::cout << "\033[35mClose connection\033[37m" << std::endl;
			break;
		}
        write(client_sock, msg, strlen(msg));
        str_len = read(client_sock, msg, BUF_SIZE - 1);
        msg[str_len] = 0;
        std::cout << "MSG from server : " << msg << std::endl;
    }
    close(client_sock);
    return 0;
}
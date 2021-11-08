#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

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
    struct timeval timeout;
    fd_set reads;
    fd_set cpy_reads;
    int fd_max;
    int fd_number;
    char buf[BUF_SIZE];

    if (ag != 2)
        error_handler("wrong argument");
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
    FD_ZERO(&reads);
    FD_SET(server_sock, &reads);
    fd_max = server_sock;

    while (true)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        if ((fd_number = select(fd_max + 1, &reads, 0, 0, &timeout)) == -1)
            break;
        if (fd_number == 0)
        {
            std::cout << "time out" << std::endl;
            continue;
        }
        for (int i = 0; i < fd_max + 1; i++)
        {
            if (FD_ISSET(i, &cpy_reads))
            {
                if (i == server_sock)
                {
                    socklen_t addr_size = sizeof(client_addr);
                    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
                    if (client_sock == -1)
                        error_handler("accept error");
                    FD_SET(client_sock, &reads);
                    if (fd_max < client_sock)
                        fd_max = client_sock;
                    std::cout << "client connected :" << client_sock << std::endl;
                }
                else
                {
                    int str_len = read(i, buf, BUF_SIZE);
                    if (str_len == 0)
                    {
                        FD_CLR(i, &reads);
                        close(i);
                        std::cout << "client closed :" << i << std::endl;
                    }
                    else
                        write(i, buf, str_len);
                }                
            }
        }
    }
    close(server_sock);
    return 0;
}
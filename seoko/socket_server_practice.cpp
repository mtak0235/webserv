#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>

void error(std::string msg);

int main(int argc, char *argv[])
{
	int server_socket;
	int client_socket;

	struct sockaddr_in server_adder;
	struct sockaddr_in client_adder;
	socklen_t clinet_adder_size;

	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
		error("socket error");
	memset(&server_adder, 0, sizeof(server_adder));
	server_adder.sin_family = AF_INET;
	server_adder.sin_addr.s_addr = htonl(INADDR_ANY);
	server_adder.sin_port = htons(atoi(argv[1]));

	if (bind(server_socket, (struct sockaddr*) &server_adder, sizeof(server_adder)) == -1)
		error("bind error");
	if (listen(server_socket, 5) == -1)
		error("listen error");
	
	clinet_adder_size = sizeof(client_adder);
	client_socket = accept(server_socket, (struct sockaddr*) &client_adder, &clinet_adder_size);
	if (client_socket == -1)
		error("accept error");
	
	char msg[] = "Hello this is server! \n";
	write(client_socket, msg, sizeof(msg));
	close(client_socket);
	close(server_socket);

	return 0;
}

void error(std::string msg)
{
	std::cout << msg << "\n";
	exit(1);
}

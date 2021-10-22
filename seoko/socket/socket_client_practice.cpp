#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error(std::string msg);

int main(int argc, char *argv[])
{
	int client_socket;
	struct sockaddr_in server_adder;
	char message[1024];

	memset(message, 1024, sizeof(message));
	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (client_socket == -1)
		error("socket error");
	
	memset(&server_adder, 0, sizeof(server_adder));
	server_adder.sin_family = AF_INET;
	server_adder.sin_addr.s_addr = inet_addr(argv[1]);
	server_adder.sin_port = htons(atoi(argv[2]));

	if (connect(client_socket, (struct sockaddr*) &server_adder, sizeof(server_adder)) == -1)
		error("connect error");
	if (read(client_socket, message, sizeof(message) - 1) == -1)
		error("read error");
	std::cout << "Message from server : " << message << "\n";

	close(client_socket);
	return 0;
}

void error(std::string msg)
{
	std::cout << msg << "\n";
	exit(1);
}
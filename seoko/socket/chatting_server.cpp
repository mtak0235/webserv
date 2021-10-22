#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024
#define INVALID_SOCKET -1

void error(std::string msg);

int main(int argc, char *argv[])
{
	char message[1024];
	int serverSocket, clientSocket;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;

	int strLen;
	socklen_t clientSize, nRcv;
	if (argc != 2)
		error("No port number");
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
		error("socket error");
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == INVALID_SOCKET)
		error("bind error");

	if (listen(serverSocket, 2) == INVALID_SOCKET)
		error("listen error");

	clientSize = sizeof(clientAddr);
	clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddr, &clientSize);
	if (clientSocket == INVALID_SOCKET)
		error("client error");
	else
	{
		std::cout << inet_ntoa(clientAddr.sin_addr) << " Connection complete!\n";
		std::cout << "Start ... \n";
	}

	close(serverSocket);
	
	while(1)
	{
		std::cout << "Message recieves .. \n";
		// data receive
		nRcv = recv(clientSocket, message, sizeof(message) - 1, 0);

		if (nRcv == INVALID_SOCKET)
		{
			std::cout << "Receive Error .... \n";
			break;
		}
		message[nRcv] = '\0';

		if (!strcmp(message, "exit"))
		{
			std::cout << "Close client connection \n";
			break;
		}

		std::cout << "Receive Message : " << message;
		std::cout << "\nsend message : ";
		std::cin >> message;

		if (!strcmp(message, "exit"))
		{
			send(clientSocket, message, (int)strlen(message), 0);
			break;
		}
		send(clientSocket, message, (int)strlen(message), 0);
	}
	close(clientSocket);
	std::cout << "Close Connection.. \n";

	return 0;
}

void error(std::string msg)
{
	std::cout << msg;
	exit(1);
}
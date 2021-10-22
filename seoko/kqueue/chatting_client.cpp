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
	int clientSocket;
	struct sockaddr_in serverAddr;

	int strLen;
	socklen_t clientSize, nRcv;
	// if (argc != 3)
	// 	error("No port number");
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		error("socket error");
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8070);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.35.204");

	if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == INVALID_SOCKET)
		error("connection error");
	else
		std::cout << "Connection Ok!\nStart ...\n";

	while(1)
	{
		std::cout << "\nSend Massage : ";
		std::cin >> message;
		if (message[0] == EOF)
			return(0);
		if (!strcmp(message, "exit"))
		{
			send(clientSocket, message, (int)strlen(message), 0);
			break;
		}

		send(clientSocket, message, (int)strlen(message), 0);
		std::cout << "Message Receives ...\n";

		nRcv = recv(clientSocket, message, sizeof(message) - 1, 0);
		if (nRcv == INVALID_SOCKET)
		{
			std::cout << "Receive Error\n";
			break;
		}
		message[nRcv] = '\0';

		if (!strcmp(message, "exit"))
		{
			std::cout << "Close server\n";
			break;
		}
		std::cout << "Receive Messages : " << message;
	}
	
	close(clientSocket);

	return 0;
}

void error(std::string msg)
{
	std::cout << msg;
	exit(1);
}
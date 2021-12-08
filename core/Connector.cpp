#include "Connector.hpp"

Connector::Connector() { }

Connector::~Connector() { }

int Connector::getSockFd(int idxServer)
{
	return _servSockFd[idxServer];
}

int Connector::connect(const std::string& portNum, int idxServer)
{
	_servSockFd[idxServer] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_servSockFd[idxServer] == CONNECTION_FAIL)
	{
		Debug::log("socket fd error");
		return CONNECTION_FAIL;
	}
	_servAddr[idxServer].sin_family = AF_INET;
	_servAddr[idxServer].sin_addr.s_addr = htonl(INADDR_ANY);
	_servAddr[idxServer].sin_port = htons(stoi(portNum));
	int	optval = 1;
	if (-1 == setsockopt(_servSockFd[idxServer], SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))) // can rebind
	{
		close(_servSockFd[idxServer]);
		return CONNECTION_FAIL;
	}
	if (bind(_servSockFd[idxServer], (struct sockaddr*) &_servAddr[idxServer],
	         sizeof(_servAddr[idxServer])) == CONNECTION_FAIL)
	{
		Debug::log("bind error " + portNum);
		return CONNECTION_FAIL;
	}
	if (listen(_servSockFd[idxServer], 1024))
	{
		Debug::log("listen error " + portNum);
		return CONNECTION_FAIL;
	}
	fcntl(_servSockFd[idxServer], F_SETFL, O_NONBLOCK);
	return CONNECTION_OK;
}

void Connector::disconnect(int idxServer)
{
	close(_servSockFd[idxServer]);
}

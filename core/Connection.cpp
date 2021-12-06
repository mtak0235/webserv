#include "Connection.hpp"

Connection::Connection() { }

Connection::~Connection() { }

int Connection::getSockFd(int i)
{
	return _servSockFd[i];
}

int Connection::connection_init(std::string port, int i)
{
	_servSockFd[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_servSockFd[i] == CONNECTION_FAIL)
	{
		_log.debugLog("socket fd error");
		return CONNECTION_FAIL;
	}
	// memset(&_servAddr[i], 0, sizeof(_servAddr[i]));
	_servAddr[i].sin_family = AF_INET;
	_servAddr[i].sin_addr.s_addr = htonl(INADDR_ANY);
	_servAddr[i].sin_port = htons(stoi(port));
	if (bind(_servSockFd[i], (struct sockaddr*) &_servAddr[i], sizeof(_servAddr[i])) == CONNECTION_FAIL)
	{
		_log.debugLog("bind error " + port);
		return CONNECTION_FAIL;
	}
	if (listen(_servSockFd[i], 1024))
	{
		_log.debugLog("listen error " + port);
		return CONNECTION_FAIL;
	}
	fcntl(_servSockFd[i], F_SETFL, O_NONBLOCK);
	return CONNECTION_OK;
}

void Connection::closeConnection(int i)
{
	close(_servSockFd[i]);
}

#include "Connection.hpp"

Connection::Connection(/* args */)
{

}

Connection::~Connection()
{

}

int Connection::getSockFd(int i)
{
	return _servSockFd[i];
}

int Connection::connection_init(std::string port, int i)
{
	_servSockFd[i] = socket(PF_INET, SOCK_STREAM, 0);
	if (_servSockFd[i] == NGX_FAIL)
	{
		_log.debugLog("socket fd error");
		return NGX_FAIL;
	}
	memset(&_servAddr[i], 0, sizeof(_servAddr[i]));
	_servAddr[i].sin_family = AF_INET;
	_servAddr[i].sin_addr.s_addr = htonl(INADDR_ANY);
	_servAddr[i].sin_port = htons(stoi(port));
	if (bind(_servSockFd[i], (struct sockaddr*) &_servAddr[i], sizeof(_servAddr[i])) == NGX_FAIL)
	{
		_log.debugLog("bind error " + port);
		return NGX_FAIL;
	}
	if (listen(_servSockFd[i], 5))
	{
		_log.debugLog("listen error " + port);
		return NGX_FAIL;
	}
	fcntl(_servSockFd[i], F_SETFL, O_NONBLOCK);
	return NGX_OK;
}

void Connection::closeConnection(int i)
{
	close(_servSockFd[i]);
}

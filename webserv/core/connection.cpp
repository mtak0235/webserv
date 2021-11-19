#include "Connection.hpp"

Connection::Connection(/* args */)
{

}

Connection::~Connection()
{

}

int Connection::connection_init(std::string port)
{
	_servSockFd = socket(PF_INET, SOCK_STREAM, 0);
	if (_servSockFd == NGX_FAIL)
	{
		_log.debug_log("socket fd error");
		return NGX_FAIL;
	}
	memset(&_servAddr, 0, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_servAddr.sin_port = htons(stoi(port));
	if (bind(_servSockFd, (struct sockaddr*) &_servAddr, sizeof(_servAddr)) == NGX_FAIL)
	{
		_log.debug_log("bind error " + port);
		return NGX_FAIL;
	}
	if (listen(_servSockFd, 1000))
	{
		_log.debug_log("listen error " + port);
		return NGX_FAIL;
	}
	return NGX_OK;
}
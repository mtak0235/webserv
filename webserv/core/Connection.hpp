#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "core.hpp"
#include "Log.hpp"

class Connection
{
private:
	int _servSockFd[10];
	struct sockaddr_in _servAddr[10];
	Log _log;
public:
	Connection();
	Connection(int& x);
	~Connection();
	int getSockFd(int i);
	int connection_init(std::string port, int i);
	void closeConnection(int i);
};



#endif

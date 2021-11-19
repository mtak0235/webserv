#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include "core.hpp"
#include "log.hpp"

class Connection
{
private:
  int _servSockFd;
	struct sockaddr_in _servAddr;
	log _log;
public:
	Connection();
	~Connection();
	int connection_init(std::string port);
};



#endif

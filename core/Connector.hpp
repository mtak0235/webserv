#ifndef CONNECTOR_HPP
# define CONNECTOR_HPP

#include <cstring>
#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Debug.hpp"

#define CONNECTION_OK 0
#define CONNECTION_FAIL -1

class Connector
{
	public:
    /* constructor & destructor */
    Connector();
    ~Connector();

    /* public function */
    int makeConnection(const std::string& portNum, int idxServer);
    void closeConnection(int idxServer);

    /* getter & setter */
    int getSockFd(int idxServer);

  private:
    /* private variable */
    struct sockaddr_in _servAddr[10];
    int _servSockFd[10];
};

#endif

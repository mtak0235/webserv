#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <cstring>
#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Log.hpp"

#define CONNECTION_OK 0
#define CONNECTION_FAIL -1

class Connection
{
	public:
    /* constructor & destructor */
    Connection();
    Connection(int& x); // 사용유무 확인 필요
    ~Connection();

    /* public function */
    int connection_init(std::string port, int i);
    void closeConnection(int i);

    /* getter & setter */
    int getSockFd(int i);

  private:
    /* private variable */
    Log _log;
    struct sockaddr_in _servAddr[10];
    int _servSockFd[10];
};

#endif

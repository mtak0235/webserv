#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_CONNECTIONS 5
#define MAX_RECIEVE 500

class Socket
{
  public:

    class SocketException : public std::exception
    {
      public:
        SocketException (const std::string& input) : _msgErr(input) {};
        ~SocketException() {};
        std::string description(void) {return _msgErr;};
        virtual const char* what(void) const throw();

      private:
        std::string _msgErr;
    };

    Socket(void);
    virtual ~Socket(void);

    bool create(void);
    bool bind(const int port);
    bool listen(void) const;
    bool accept(Socket& newSocket) const;
    bool send(const std::string& msg) const;
    int receive(std::string& msg) const;
    bool connect(const std::string& host, const int& port);

    void setNonBlocking(const bool& isNonBlock);

  private:

    int _fdSocket;
    sockaddr_in _addr;
};

#endif

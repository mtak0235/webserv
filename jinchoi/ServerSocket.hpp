#ifndef Server_Socket_HPP
#define Server_Socket_HPP

#include "Socket.hpp"

class ServerSocket : private Socket
{
  public:

    ServerSocket(const int& port);
    ServerSocket() {};
    virtual ~ServerSocket();

    const ServerSocket& operator << (const std::string& msg) const;
    const ServerSocket& operator >> (std::string& msg) const;

    void accept(ServerSocket& other);
};

#endif

#ifndef Client_Socket_HPP
#define Client_Socket_HPP

#include "Socket.hpp"

class ClientSocket : private Socket
{
  public:
    ClientSocket(const std::string& host, const int& port);
    virtual ~ClientSocket() {};

    const ClientSocket& operator << (const std::string& msg) const;
    const ClientSocket& operator >> (std::string& msg) const;
};

#endif

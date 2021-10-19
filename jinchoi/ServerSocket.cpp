#include "ServerSocket.hpp"

ServerSocket::ServerSocket(const int& port)
{
  if (!Socket::create())
    throw Socket::SocketException("Failed to create server socket.\n");
  if (!Socket::bind(port))
    throw Socket::SocketException("Failed to bind port.\n");
  if (!Socket::listen())
    throw Socket::SocketException("Failed to listen to socket.\n");
}

ServerSocket::~ServerSocket() {}

const ServerSocket& ServerSocket::operator << (const std::string& msg) const
{
  if(!Socket::send(msg))
    throw Socket::SocketException("Failed to write on socket.\n");
  return *this;
}

const ServerSocket& ServerSocket::operator >> (std::string& msg) const
{
  if (!Socket::receive(msg))
    throw Socket::SocketException("Failed to read on socket.\n");
  std::cout << msg << std::endl;
  return *this;
}

void ServerSocket::accept(ServerSocket& other)
{
  if (!Socket::accept(other))
    throw Socket::SocketException("Failed to accept socket.\n");
}

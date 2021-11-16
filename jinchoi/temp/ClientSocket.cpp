#include "ClientSocket.hpp"

ClientSocket::ClientSocket(const std::string& host, const int& port)
{
  if (!Socket::create())
    throw Socket::SocketException("Failed to create client socket.\n");
  if (!Socket::connect(host, port))
    throw Socket::SocketException("Failed to bind port");
}

const ClientSocket& ClientSocket::operator << (const std::string& msg) const
{
  if (!Socket::send(msg))
    throw Socket::SocketException("Failed to write msg to socket");;
  return *this;
}

const ClientSocket& ClientSocket::operator >> (std::string& msg) const
{
  if (!Socket::receive(msg))
    throw Socket::SocketException("Failed to read msg from socket.\n");
  return *this;
}

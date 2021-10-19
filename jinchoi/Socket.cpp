#include "Socket.hpp"

const char* Socket::SocketException::what(void) const throw()
{
  return "Socket::SocketException";
}

Socket::Socket(void)
    : _fdSocket(-1)
{
  memset(&_addr, 0, sizeof(_addr));
}

Socket::~Socket(void)
{
  if (_fdSocket != -1)
    close(_fdSocket);
}

bool Socket::create(void)
{
  _fdSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (_fdSocket == -1)
    return false;
  int opt = 1;
  if (setsockopt(_fdSocket, SOL_SOCKET, SO_REUSEADDR,
                 (const char*)&opt, sizeof(opt)) == -1)
    return false;
  return true;
}

bool Socket::bind(const int port)
{
  if (_fdSocket == -1)
    return false;

  _addr.sin_family = AF_INET;
  _addr.sin_addr.s_addr = INADDR_ANY;
  _addr.sin_port = htons(port);

  int retBind = ::bind(_fdSocket, (struct sockaddr*)&_addr, sizeof(_addr));
  if (retBind == -1)
    return false;
  return true;
}

bool Socket::listen(void) const
{
  if (_fdSocket == -1)
    return false;
  int retListen = ::listen(_fdSocket, MAX_CONNECTIONS);
  if (retListen == -1)
    return false;
  return true;
}

bool Socket::accept(Socket& newSocket) const
{
  socklen_t lenAddr = sizeof(_addr);
  newSocket._fdSocket = ::accept(_fdSocket, (sockaddr*)&_addr, &lenAddr);
  if (newSocket._fdSocket <= 0)
    return false;
  return true;
}

bool Socket::send(const std::string& msg) const
{
  int retSend = ::send(_fdSocket, msg.c_str(), msg.size(), MSG_NOSIGNAL);
  if (retSend == -1)
    return false;
  return true;
}

int Socket::receive(std::string& msg) const
{
  char buffer[MAX_RECIEVE + 1] = {0, };
  msg = "";
  int retRecv = recv(_fdSocket, buffer, MAX_RECIEVE, 0);
  if (retRecv == -1)
  {
    std::cout << "Failed to receive.\n" << errno << "\n";
    return 0;
  }
  else if (!retRecv)
    return 0;
  else
  {
    msg = buffer;
    return retRecv;
  }
}

bool Socket::connect(const std::string& host, const int& port)
{
  if (_fdSocket == -1)
    return false;
  _addr.sin_family = AF_INET;
  _addr.sin_port = htons(port);
  int binNetwork = inet_pton(AF_INET, host.c_str(), &_addr.sin_addr);
  if (errno == EAFNOSUPPORT)
    return false;
  binNetwork = ::connect(_fdSocket, (sockaddr*)&_addr, sizeof(_addr));
  if (binNetwork == -1)
    return false;
  return true;
}

void Socket::setNonBlocking(const bool& isNonBlock)
{
  int status = fcntl(_fdSocket, F_GETFL);
  if (status < 0)
    return ;
  if (isNonBlock)
    status |= O_NONBLOCK;
  else
    status &= ~O_NONBLOCK;
  fcntl(_fdSocket, F_SETFL, status);
}

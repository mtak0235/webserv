#include <iostream>
#include <string>

#include "ClientSocket.hpp"

int main(int argc, char* argv[])
{
  try
  {
    const std::string nameHost = argv[1];
    ClientSocket cliSocket(nameHost, 30000);
    std::string reply;
    try
    {
      cliSocket << "Host : " << nameHost;
      cliSocket >> reply;
    }
    catch (const Socket::SocketException& ) {}
    std::cout << "Received msg : [" << reply << "]\n";
  }
  catch (Socket::SocketException e)
  {
    std::cout << e.description() << "\n";
  }
  return 0;
}

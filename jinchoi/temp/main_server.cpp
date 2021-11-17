#include <string>
#include <iostream>

#include "ServerSocket.hpp"

int main(int argc, char* argv[])
{
  std::cout << "running ... \n";
  try {
    ServerSocket server(30000);
    while (true) {
      ServerSocket newSocket;
      server.accept(newSocket);
      try {
        while (true) {
          std::string data;
          newSocket >> data;
          newSocket << data;
        }
      }
      catch (Socket::SocketException&) {}
    }
  }
  catch (Socket::SocketException& exception) {
    std::cout << exception.description() << "\n";
  }
  return 0;
}

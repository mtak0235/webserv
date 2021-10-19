#include <sys/socket.h> // socklen_t type, AF_INET def, SOCK_STREAM def
#include <netinet/in.h> // sockaddr_in type
#include <arpa/inet.h> // inet_ntoa func

#include <stdio.h>
#include <stdlib.h> // exit func
#include <unistd.h> // read func, close func
#include <string.h> // bzero func

void handleError(const char* msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char* argv[])
{
  if (argc < 2)
    handleError("ERROR, no port provided\n");

  int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_socket < 0)
    handleError("ERROR, failed to opening socket.\n");

  int num_port = atoi(argv[1]);

  struct sockaddr_in addr_serv;
  bzero((char*)&addr_serv, sizeof(addr_serv)); // clear
  addr_serv.sin_family = AF_INET; // setup for bind call
  addr_serv.sin_addr.s_addr = INADDR_ANY; // automatically fill current host's IP addr
  addr_serv.sin_port = htons(num_port); // convert int to network byte order

  if (bind(fd_socket, (struct sockaddr*)&addr_serv, sizeof(addr_serv) < 0))
    handleError("ERROR, failed to binding.\n");

  listen(fd_socket, 5); // until accept, 2nd arg is size for backlog queue

  struct sockaddr_in addr_cli;
  socklen_t len_cli = sizeof(addr_cli);
  int fd_new_socket = accept(fd_socket, (struct sockaddr*)&addr_cli, &len_cli);
  if (fd_new_socket < 0)
    handleError("ERROR, failed to accepting.\n");

  printf("Server : got connection from %s port %d\n",
         inet_ntoa(addr_cli.sin_addr), ntohs(addr_cli.sin_port));

  send(fd_new_socket, "Hello, world!\n", 13, 0);

  char buffer[256] = {0, };
  int bytes_read = read(fd_new_socket, buffer, 255);
  if (bytes_read < 0)
    handleError("ERROR, failed to reading from socket.\n");

  printf("Here is the message : %s\n", buffer);

  close(fd_new_socket);
  close(fd_socket);
  return 0;
}

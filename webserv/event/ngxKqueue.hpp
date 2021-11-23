#ifndef NGX_KQUEUE_HPP
#define NGX_KQUEUE_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <sys/event.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/event.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "Log.hpp"
#include "core.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "Cluster.hpp"
// #include "Server.hpp"

class ngxKqueue : public Cluster
{
private:
  int _newEvents[6];
  int _kq;
  struct kevent _eventList[1024];
protected:
	std::vector<struct kevent> _changeList;
	std::map<int, std::string> _clients;
	struct kevent *_currEvent;
public:
  ngxKqueue();
  ~ngxKqueue();
  ngxKqueue(ngxKqueue& x);
  // ngxKqueue operator=(ngxKqueue& x);

  void changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
  void disconnectClient(int clientFd, std::map<int, std::string>& clients);
  int makeKqueue();
  void ngxKqueueInit(int servSock);
  int ngxKqueueProcessEvents(int x, std::vector<int> servSock);
  void ngxKqueueStop();

	virtual void acceptNewClient(int servSock) = 0;
	virtual int recvDataFromClient(int k) = 0;
};

#endif

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

class ngxKqueue
{
private:
  int _newEvents[6];
  int _kq;
  std::map<int, std::string> _clients;
  std::vector<struct kevent> _changeList;
  struct kevent _eventList[1024];
  struct kevent *_currEvent;
  Log _log;

  std::vector<ServerConfig> _serverConfigs;
public:
  ngxKqueue();
  ~ngxKqueue();
  ngxKqueue(ngxKqueue& x);
  ngxKqueue operator=(ngxKqueue& x);

  void changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
  void disconnectClient(int clientFd, std::map<int, std::string>& clients);
  int makeKqueue(std::vector<ServerConfig> &_serverConfigs);
  void ngxKqueueInit(int servSock);
  int ngxKqueueProcessEvents(int x, std::vector<int> servSock);
  void ngxKqueueStop();
};

#endif

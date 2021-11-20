#ifndef NGX_KQUEUE_HPP
#define NGX_KQUEUE_HPP

#include <iostream>
#include <vector>
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
#include "log.hpp"
#include "core.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"

class ngx_kqueue
{
private:
  int _changes;
  int _events;
  int _kq;
  std::map<int, std::string> _clients;
  std::vector<struct kevent> _change_list;
  struct kevent _event_list[1024];
  struct kevent *_curr_event;
  log _log;

  std::vector<ServerConfig> _serverConfigs;
public:
  ngx_kqueue();
  ~ngx_kqueue();
  ngx_kqueue(ngx_kqueue& x);
  ngx_kqueue operator=(ngx_kqueue& x);

  void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
  void disconnect_client(int client_fd, std::map<int, std::string>& clients);
  int make_kqueue(std::vector<ServerConfig> &_serverConfigs);
  void ngx_kqueue_init(int servSock);
  int ngx_kqueue_process_events(int x, int servSock);
  void ngx_kqueue_stop();
};

#endif

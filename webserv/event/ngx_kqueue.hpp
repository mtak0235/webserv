#ifndef NGX_KQUEUE_HPP
#define NGX_KQUEUE_HPP

#include <iosteream>
#include <map>

class ngx_kqueue
{
private:
  unsigned int changes;
  unsigned int events;
  map<int, string> clients;
  vector<struct kevent> change_list;
  struct kevent event_list[1024];
public:
  ngx_kqueue();
  ~ngx_kqueue();
  ngx_kqueue(ngx_kqueue& x);
  ngx_kqueue operator=(ngx_kqueue& x);

  ngx_kqueue_init();
  ngx_kqueue_process_events();
};

#endif

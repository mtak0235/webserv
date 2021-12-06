#include "ngxKqueue.hpp"

ngxKqueue::ngxKqueue() { }

ngxKqueue::~ngxKqueue() { }

void ngxKqueue::ngxKqueueInit(int servSock)
{
  changeEvents(_changeList, servSock, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

int ngxKqueue::ngxKqueueProcessEvents(int x, std::vector<int> servSock)
{
  for (int i = 0; i < x; i++)
  {
    _newEvents[i] = kevent(_kq, &_changeList[i], _changeList.size(), _eventList, 1024, NULL);
    if (_newEvents[i] == -1)
    {
      Debug::log("kevent error");
      return FAIL;
    }
  }
  _changeList.clear();
  for (int k = 0; k < x; k++)
  {
    for (int i = 0; i < _newEvents[k]; ++i)
    {
      _currEvent = &_eventList[i];
      /* check error event return */
      if (_currEvent->flags & EV_ERROR)
      {
        if (_currEvent->ident == (uintptr_t)servSock[k])
        {
          Debug::log("socket error");
          return FAIL;
        }
        else
          disconnectClient(_currEvent->ident, _clients);
      }
      else if (_currEvent->filter == EVFILT_READ)
      {
        if (_currEvent->ident == (uintptr_t)servSock[k])
          acceptNewClient(servSock[k]);
        else if (_clients.find(_currEvent->ident)!= _clients.end())
        {
          if (recvDataFromClient(k) == FAIL)
            return FAIL;
        }
      }
    }
  }
  return SUCCESS;
}

void ngxKqueue::disconnectClient(int clientFd, std::map<int, std::string>& clients)
{
  close(clientFd);
  clients.erase(clientFd);
}

// 헤더에 있는 void ngxKqueueStop(void); // 구현안댐 사용유무 확인 필요~~

void ngxKqueue::changeEvents(std::vector<struct kevent>& changeList,
                             uintptr_t ident, int16_t filter, uint16_t flags,
                             uint32_t fflags, intptr_t data, void *udata)
{
  struct kevent tempEvent;
  EV_SET(&tempEvent, ident, filter, flags, fflags, data, udata);
  changeList.push_back(tempEvent);
}

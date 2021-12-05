#ifndef NGX_KQUEUE_HPP
# define NGX_KQUEUE_HPP

#include "Cluster.hpp"

class ngxKqueue : public Cluster
{
  public:
    /* constructor & destructor */
    ngxKqueue();
    virtual ~ngxKqueue();

    /* public function */
    virtual int makeKqueue(void);
    virtual void ngxKqueueInit(int servSock);
    virtual int ngxKqueueProcessEvents(int x, std::vector<int> servSock);
    void disconnectClient(int clientFd, std::map<int, std::string>& clients);
    void ngxKqueueStop(void); // ************** 구현안댐 사용유무 확인 필요~~
    void changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident,
                      int16_t filter, uint16_t flags, uint32_t fflags,
                      intptr_t data, void *udata);

    /* pure virtual function */
    virtual void acceptNewClient(int servSock) = 0;
    virtual int recvDataFromClient(int k) = 0;

  protected:
    /* protected variable */
    std::vector<struct kevent> _changeList;
    std::map<int, std::string> _clients;
    struct kevent* _currEvent;

  private:
    /* private variable */
    int _kq;
    int _newEvents[6];
    struct kevent _eventList[1024];
};

#endif

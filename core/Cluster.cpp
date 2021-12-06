#include "Cluster.hpp"

Cluster::Cluster(const std::string& configFile)
    : _configFile(configFile) {
  _makeStatusMap();
}

Cluster::~Cluster() { }

void Cluster::_makeStatusMap(void)
{
  _statusMap[200] = "OK";
  _statusMap[300] = "Multiple Choice";
  _statusMap[302] = "Found";
  _statusMap[400] = "Bad Request";
  _statusMap[403] = "Forbidden";
  _statusMap[404] = "Not Found";
  _statusMap[405] = "Method Not Allowed";
  _statusMap[500] = "Internal Server Error";
}

int Cluster::_makeServerInfo(const std::string& configFile) {
  if (_parser.parse(configFile) == FAIL) {
    Debug::log("No correct config file");
    return FAIL;
  }
  _serverInfos = _parser.getServerConfig();
}

int Cluster::_makeKqueue(void)
{
  _FdEventQueue = kqueue();
  if (_FdEventQueue == FAIL) {
    Debug::log("kqueue() error");
    closeAllConnection();
    return FAIL;
  }
  size_t cntServer = _serverInfos.size();
  for (size_t idxServer = 0; idxServer < cntServer; idxServer++)
    _changeEvents(_changeList, _connector.getSockFd(idxServer), EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
  return SUCCESS;
}

void Cluster::_changeEvents(std::vector<struct kevent>& changeList,
                             uintptr_t ident, int16_t filter, uint16_t flags,
                             uint32_t fflags, intptr_t data, void *udata)
{
  struct kevent tempEvent;
  EV_SET(&tempEvent, ident, filter, flags, fflags, data, udata);
  changeList.push_back(tempEvent);
}

int Cluster::_makeConnection(void) {
  size_t cntServer = _serverInfos.size();
  for (size_t idxServer = 0; idxServer < cntServer; idxServer++) {
    if (_connector.makeConnection(_serverInfos[idxServer].getServerPort(), idxServer) == FAIL) {
      closeAllConnection();
      return FAIL;
    }
  }
}

int Cluster::init(void)
{
  _makeStatusMap();
  if (_makeServerInfo(_configFile) == FAIL)
    return FAIL;
  if (_makeConnection() == FAIL)
    return FAIL;
  if (_makeKqueue() == FAIL)
    return FAIL;
  return SUCCESS;
}


int Cluster::run(void)
{
  std::vector<int> fdServerSocket;
  size_t cntServer = _serverInfos.size();
  for (size_t idxServer = 0; idxServer < cntServer; idxServer++)
    fdServerSocket.push_back(_connector.getSockFd(idxServer));
  while (true) {
    if (ngxKqueueProcessEvents(cntServer, fdServerSocket) == FAIL) {
      closeAllConnection();
      return FAIL;
    }
  }
  return SUCCESS;
}

int Cluster::closeAllConnection(void)
{
  size_t cntServer = _serverInfos.size();
  for (size_t idxServer = 0; idxServer < cntServer; idxServer++)
    _connector.closeConnection(idxServer);
  return SUCCESS;
}

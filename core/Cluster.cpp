#include "Cluster.hpp"

Cluster::Cluster() { }

Cluster::~Cluster() { }

int Cluster::init(const std::string& confFile)
{
  if (_parser.parse(confFile) == NGX_FAIL)
  {
    _log.debugLog("No correct config file");
    return NGX_FAIL;
  }
  _serverConfigs = _parser.getServerConfig();
  setStatus();
  int n = _serverConfigs.size();
  for (int i = 0; i < n; i++)
  {
    if (_connection.connection_init(_serverConfigs[i].getServerPort(), i) == NGX_FAIL)
    {
      stop();
      return NGX_FAIL;
    }
  }
  if (makeKqueue() == NGX_FAIL)
  {
      stop();
      return NGX_FAIL;
  }
  for (int i = 0; i < n; i++)
    ngxKqueueInit(_connection.getSockFd(i));
  return NGX_OK;
}

int Cluster::run(void)
{
  std::vector<int> servSock;
  for (unsigned long i = 0; i < _serverConfigs.size(); i++)
    servSock.push_back(_connection.getSockFd(i));
  while (true)
  {
    if (ngxKqueueProcessEvents(_serverConfigs.size(), servSock) == NGX_FAIL)
    {
      stop();
      return NGX_FAIL;
    }
  }
  return NGX_OK;
}

int Cluster::stop(void)
{
  int n = _serverConfigs.size();
  for (int i = 0; i < n; i++)
    _connection.closeConnection(i);
  return NGX_OK;
}

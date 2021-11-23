#include "Cluster.hpp"

Cluster::Cluster(void)
{
}

Cluster::~Cluster()
{
}

int Cluster::init(const std::string& confFile) 
{
	_parser.parse(confFile);
	_serverConfigs = _parser.getServerConfig();
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
	while (1)
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
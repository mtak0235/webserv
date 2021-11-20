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
		if (_connection.connection_init(_serverConfigs[i].serverPort, i) == NGX_FAIL)
			return NGX_FAIL;
	}
	if (_ngx_kqueue.make_kqueue(_serverConfigs) == NGX_FAIL)
		return NGX_FAIL;
	for (int i = 0; i < n; i++)
		_ngx_kqueue.ngx_kqueue_init(_connection.getSockFd(i));
	return NGX_OK;
}

int Cluster::run(void) 
{
	while (1)
	{
		for (int i = 0; i < _serverConfigs.size(); i++)
		{
			if (_ngx_kqueue.ngx_kqueue_process_events(i, stoi(_serverConfigs[i].serverPort)) == NGX_FAIL)
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
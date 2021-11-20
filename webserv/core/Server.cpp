#include "Server.hpp"

Server::Server() {

}

Server::~Server() {

}

int Server::init(std::string confFile)
{
	_parser.parse(confFile);
	_serverConfigs = _parser.getServerConfig();
	int n = _serverConfigs.size();
	for (int i = 0; i < n; i++)
	{
		if (connection.connection_init(_serverConfigs[i].serverPort) == -1)
			return NGX_FAIL;
	}
	return 0;
}

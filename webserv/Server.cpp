#include "Server.hpp"

Server::Server() {

}

Server::~Server() {

}

int Server::init(std::string confFile)
{
	_parser.parse(confFile);
	_serverConfigs = _parser.getServerConfig();
	return 0;
}

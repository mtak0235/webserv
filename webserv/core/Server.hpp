#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include "Config.hpp"
#include "Parser.hpp"
#include "Connection.hpp"
#include "core.hpp"

class Server
{
	public :
		Server();
		~Server();
		int init(std::string confFile);
		int run();
		int stop();

	private :
		Parser _parser;
		Connection connection;
		int _initConnection();
		std::vector<ServerConfig> _serverConfigs;
};

#endif

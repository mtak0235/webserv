#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include "Config.hpp"
#include "Parser.hpp"

class Server
{
	public :
		Server();
		~Server();
		int init(std::string confFile);
		int run();
		int stop();

	private :
		ServerConfig _sc;
		Parser _parser;
		int _initConnection();
		std::vector<ServerConfig> _serverConfigs;
};

#endif

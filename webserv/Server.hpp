#include <vector>
#include <string>

#include "Parser.hpp"

struct LocationConfig
{
	std::string locationName;
	std::string root;
	std::vector<std::string> indexList;
	std::string allowMethod;
	std::string cgiName;
	std::string cgiPath;
	int cliBodySize;
	std::string uploadFolder;
};

struct ServerConfig
{
	std::string serverName;
	int serverPort;
	std::vector<LocationConfig> locations;
};

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

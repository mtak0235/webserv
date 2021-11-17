#include "Config"

#define ERROR -1
#define OK 0;

class Server
{
	typedef Config::ServerConfig ServerConfig;

	public :
		int init();
		int run();
		int stop();
	private :
		ServerConfig _sc;
		int initConnection();
		struct ServerConfig
    {
      std::string serverName;
      int serverPort;
      struct LocationConfig
      {
        std::string root;
        std::vector<std::string> indexList;
        std::string allowMethod;
        std::string cgiPath;
        int cliBodySize;
      };
      std::vector<LocationConfig> locationConfigs;
    };
}

int Server::init(std::string path)
{
	Parser c;
	
	c.parse(path);
	_sc = c.getServerConfig();
	if (initConnection() == -1)
		return FAIL;
	

}
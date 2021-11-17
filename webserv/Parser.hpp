#ifndef Parser_HPP
# define Parser_HPP

#include <string>
#include <vector>

class Parser
{

  public:
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

    Parser();
    ~Parser();

    void parse(void);
    std::vector<ServerConfig> getServerConfig(void) const;
    
  private:
    std::vector<ServerConfig> _serverConfigs;
    ServerConfig _ParseServerConfig(void);
};

#endif
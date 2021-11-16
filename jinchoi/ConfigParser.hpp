#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

#include <string>
#include <vector>

class ConfigParser
{
  public:
    struct Server
    {
      std::string serverName;
      int serverPort;
      struct Location
      {
        std::string root;
        std::vector<std::string> indexList;
        std::string allowMethod;
        std::string cgiPath;
        int cliBodySize;
      };
      std::vector<Location> locations;
    };

  public:

    ConfigParser();
    ~ConfigParser();

    void parse(void);
    std::vector<Server> getServerConfig(void) const;
    
  private:
    std::vector<Server> _servers;
    Server _ParseServerConfig(void);
};

#endif
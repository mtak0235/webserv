#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Config.hpp"

class Parser
{
  enum KEY_SERVER {
    LISTEN,
    SERVER_NAME,
    LOCATION,
  };

  enum KEY_LOCATION {
    BODY_SIZE,
    METHOD,
    INDEX,
    ROOT,
    CGI_EXTENSION,
    CGI_PATH,
    UPLOAD_FOLDER,
  };

  public:
    Parser();
    ~Parser();

    void parse(const std::string& confFile);
    std::vector<ServerConfig> getServerConfig(void) const;
    
  private:
      static const std::string _keyInit;
      static const std::string _keyServer[3];
      static const std::string _keyLocation[7];
      
      std::ifstream _ifs;
      std::string _info;
  	  ServerConfig _parseServerBlock();
	  LocationConfig _parseLocationBlock();
      std::vector<ServerConfig> _serverConfigs;
};

#endif

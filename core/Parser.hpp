#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Config.hpp"
#include "Core.hpp"
#include "Log.hpp"

class Parser
{
  enum KEY_SERVER {
    LISTEN,
    SERVER_NAME,
    LOCATION
  };

  enum KEY_LOCATION {
    AUTO_INDEX,
    BODY_SIZE,
    METHOD,
    INDEX,
    ROOT,
    CGI_EXTENSION,
    CGI_PATH,
    UPLOAD_FOLDER,
		REDIRECTION
  };

  public:
    Parser();
    ~Parser();

    int parse(const std::string& confFile);
    std::vector<ServerConfig> getServerConfig(void) const;
  private:
		static const std::string _keyInit;
		static const std::string _keyServer[3];
		static const std::string _keyLocation[9];
		Log _log;

		int _getLocationInfo(std::stringstream &ss, std::string msg);
		std::ifstream _ifs;
		std::string _info;
		bool _checkSemicolon(char c);
		ServerConfig _parseServerBlock();
		LocationConfig _parseLocationBlock();
	  int _locationRemoveSemicolon(LocationConfig *lc);
	  int _serverRemoveSemicolon(ServerConfig *sc);
		std::vector<ServerConfig> _serverConfigs;
};

#endif

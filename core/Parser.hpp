#ifndef PARSER_HPP
# define PARSER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Debug.hpp"
#include "Config.hpp"

# define PARSE_FAIL -1
# define PARSE_OK 0

class Parser
{
  /* enumerator */
  enum KEY_SERVER
  {
    LISTEN,
    SERVER_NAME,
    LOCATION
  };
  enum KEY_LOCATION
  {
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
    /* constructor & destructor */
    Parser();
    ~Parser();

    /* public static function */
    static const std::string getConfigFile(int argc, char* argv[]);

    /* public function */
    int parse(const std::string& confFile);

    /* getter & setter */
    std::vector<ServerConfig> getServerConfig(void) const;

  private:
    /* private static variable */
    static const std::string _keyInit;
    static const std::string _keyServer[3];
    static const std::string _keyLocation[9];

    /* private variable */
    std::ifstream _ifs;
    std::string _info;
    std::vector<ServerConfig> _serverInfos;

    /* private function */
    int _getLocationInfo(std::stringstream &ss, std::string msg);
};

#endif

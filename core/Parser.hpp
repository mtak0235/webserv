#ifndef PARSER_HPP
# define PARSER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Debug.hpp"
#include "Config.hpp"

# define PARSE_FAIL -1 // core.hpp 삭제하면서 ngx_OK 랑 중복 define 이라 이름 바꿈
# define PARSE_OK 0  // core.hpp 파일은 내용이 ngx_ok 랑 ngx_fail 만 있었어서 지움

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
    static const std::string _keyServer[3]; // 하드코딩 3 빼기
    static const std::string _keyLocation[9]; // 하드코딩 9 빼기

    /* private variable */
    std::ifstream _ifs;
    std::string _info;
    std::vector<ServerConfig> _serverConfigs;

    /* private function */
    int _getLocationInfo(std::stringstream &ss, std::string msg);
};

#endif

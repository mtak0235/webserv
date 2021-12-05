#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <ctime>
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <sstream>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../core/Formatter.hpp"

class Response
{
  public:
    /* constructor & destructor */
    Response();
    ~Response();

    /* public function */
    std::string makeResponse(const std::string& body);
    const std::string	generateAutoindexPage(std::string const &path) const;

    /* getter & setter */
    void setStatusCode(const int& n);
    void setStatusMsg(const std::string& str);
    void setServerName(const std::string& str);
    void setContentType(std::string type);
    void setLocation(const std::string& str);

  private:
    /* private static variable */
    static const std::string _httpVersion;

    /* private variable */
    int _statusCode;
    std::string _statusMsg;
		std::string _contentType;
    std::string _location;
    std::string _serverName;

    /* private static function */
    static std::string _getFormattedNum(const int& num);
    static std::string _getDate(void);

    /* private functions */
    std::string _makeStatusLine(void);
    std::string _makeHeader(void);
    std::string _makeLocation(void);
    std::string _makeServerInfo(void);
};
#endif

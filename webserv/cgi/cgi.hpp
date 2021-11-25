#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <fstream>
#include <unistd.h>

#include "Request";

class Request;

class Cgi
{
  enum ENVIRON_LIST {
    AUTH_TYPE,
    CONTENT_LENGTH,
    CONTENT_TYPE,
    GATEWAY_INTERFACE,
    PATH_INFO,
    PATH_TRANSLATED,
    QUERY_STRING,
    REMOTE_ADDR,
    REMOTE_IDENT,
    REMOTE_USER,
    REQUEST_METHOD,
    REQUEST_URI,
    SCRIPT_FILENAME,
    SERVER_NAME,
    SERVER_PORT,
    SERVER_PROTOCOL,
    SERVER_SOFTWARE,
    REDIRECT_STATUS,
    NON_OF_ALL,
  };

  public:
    Cgi(void);
    ~Cgi();
    
    std::string getCgiResponse(Request req);
  
  private:
    static const std::string _environList[NON_OF_ALL];

    void _setEnviron(const Request& req);
    size_t _getEnvironListSize(const Request& req) const;
    const std::string _getCwd(const std::string& path) const;
    int _statusCode;
    std::string _statusMsg;
    std::string _body;
    char** _environ;

}

#endif


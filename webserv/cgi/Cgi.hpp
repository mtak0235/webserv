#ifndef CGI_HPP
#define CGI_HPP

#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

#include <cstring>
#include <string>
#include <fstream>
#include <map>

#include "Request.hpp"


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

    std::string execute(Request req, std::string cgiFilePath, std::string file);
    std::string getCgiResponseBody();
    std::string getCgiResponseHeader();
		int getStatusCode();

  private:
    static const std::string _environList[NON_OF_ALL];

    void _setEnviron(const Request& req);
		std::string _setBody(std::string file);
    std::map<std::string, std::string> _makeEnvMap(const Request& req) const;
    const std::string _getCwd(void) const;
    // int _statusCode;
    // std::string _statusMsg;
    // std::string _body;
    char** _environ;
    size_t _allocSize;
    std::string _cgiResponseHeader;
    std::string _cgiResponseBody;
    void _parseCgiResponse(std::string str);
		int _statusCode;
};

#endif

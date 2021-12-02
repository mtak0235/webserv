#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <ctime>
#include <string>
#include <sstream>

#include "../core/Formatter.hpp"

class Response
{
  public:
    Response(void);
    ~Response();

    std::string makeResponse(const std::string& body);

    void setStatusCode(const int& n);
    void setStatusMsg(const std::string& str);
    void setServerName(const std::string& str);
    // void setContentLength(const int& n);
    void setContentType(std::string type);
  private:
    static const std::string _httpVersion;

    static std::string _getFormattedNum(const int& num);
    static std::string _getDate(void);


    std::string _makeStatusLine();
    std::string _makeHeader();

    std::string _makeServerInfo(void);

    int _statusCode;
    std::string _statusMsg;
		std::string _contentType;
    std::string _serverName;
    // int _contentLength;
};
#endif

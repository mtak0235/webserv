#include <ctime>
#include <string>
#include <sstream>

class Response
{
  public:
    Response(void);
    ~Response();

    std::string makeResponse(const std::string& body);

    void setStatusCode(const int& n);
    void setStatusMsg(const std::string& str);
    void setServerName(const std::string& str);

  private:
    static const std::string _httpVersion;
    static std::string _getFormattedNum(const int& num);

    std::string _makeStatusLine();
    std::string _makeHeader();

    int _statusCode;
    std::string _statusMsg;

    std::string _serverName;
    int _contentLength;
};

// [HEADER]
// string server_name
// string date
// int content_length
// string content_type
// string allow_method

// [BODY]
// string body

#include <string>
#include <sstream>

class Response
{
  public:
    Response(void);
    ~Response();

    std::string makeResponse();

    void setHttpVersion(const std::string& str);
    void setStatusCode(const int& n);
    void setStatusMsg(const std::string& str);

  private:
    std::string _makeStatusLine();
    std::string _makeHeader();
    std::string _makeBody();

    std::string _httpVersion;
    int _statusCode;
    std::string _statusMsg;

    int _contentLength;
};

// [STATUS LINE]
// int HTTP_version
// int status_code
// string status_msg

// [HEADER]
// int content_length
// string server_name
// string date
// string content_type
// string allow_method

// [BODY]
// string body

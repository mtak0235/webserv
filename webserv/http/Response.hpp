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

  private:
    static const std::string _httpVersion;

    std::string _makeStatusLine();
    std::string _makeHeader();

    int _statusCode;
    std::string _statusMsg;
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

#include <string>
#include <map>
#include <cstring>
enum http_method
{
    GET= 0 ,
    POST, 
    DELETE
};

class Request
{
    private:
        std::string _raw;
        std::string _rawFirstLine;
        http_method _httpMethod;
        std::string _path;
        int _httpVersion;
        std::string _rawHeader;
        std::map<std::string, std::string> _headers;
        std::string _body;

    public:
        Request();
        virtual ~Request();
        http_method getHttpMethod();
        std::string getPath();
        std::map<std::string, std::string> getHeaders();
        std::string getBody();
        void parseRequest(std::string);
};

Request::Request(/* args */)
{
    
}

Request::~Request()
{

}

http_method Request::getHttpMethod()
{
    return this->_httpMethod;
}
std::string Request::getPath()
{
    return this->_path;
}
std::map<std::string, std::string> Request::getHeaders()
{
    return this->_headers;
}
std::string Request::getBody()
{
    return this->_body;
}

void Request::parseRequest(std::string request)
{
    try
    {
        this->_rawFirstLine = strtok((char *)request.c_str(), "\n");
        this->_rawHeader = strtok((char *)request.c_str(), "\n");
        char *headerKey;
        char *headerValue;
        headerKey = 
        this->_headers.insert({strtok((char *)this->_rawHeader.c_str(), ": "), });
        while()
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
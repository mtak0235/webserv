#include <string>
#include <map>
#include <cstring>
#include <iostream>
#include <string>

enum HTTP_METHOD
{
    GET,
    POST, 
    DELETE
};

class Request
{
    private:
        std::string _raw;
        std::string _rawFirstLine;
        HTTP_METHOD _httpMethod;
        std::string _path;
        std::string _httpVersion;
        std::string _rawHeader;
        std::map<std::string, std::string> _headers;
        std::string _body;

    public:
        Request();
        virtual ~Request();
        HTTP_METHOD getHttpMethod();
        std::string getPath();
        std::map<std::string, std::string> getHeaders();
        std::string getBody();
        void parseRequest(std::string);
        void setHttpMethod(std::string);
        void setPath(std::string);
        void setHttpVersion(std::string);
};



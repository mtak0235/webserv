#include "Request.hpp"

Request::Request(/* args */)
{
    
}

Request::~Request()
{

}

HTTP_METHOD Request::getHttpMethod()
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

void Request::setHttpMethod(std::string method)
{
    if (!strncmp(method.c_str(), "GET ", 4))
		this->_httpMethod = GET;
	else if (!strncmp(method.c_str(), "POST ", 5))
		this->_httpMethod = POST;
	else if (!strncmp(method.c_str(), "DELETE ", 6))
		this->_httpMethod = DELETE;
}

void Request::setPath(std::string path)
{
    this->_path = path;
}

void Request::setHttpVersion(std::string version)
{
    this->_httpVersion = version;
}
void Request::parseRequest(std::string request)
{
    try
    {
        char *headerKey;
        char *headerValue;

        setHttpMethod(strtok((char *)request.c_str(), " "));
        setPath(strtok((char *)request.c_str(), " HTTP/"));
        setHttpVersion(strtok((char *)request.c_str(), "\n"));
        // std::cout << this->_httpMethod << this->_path<< this->_httpVersion << std::endl;
        while (headerValue != NULL || headerValue == "")
        {
            headerKey = strtok(NULL, ": ");
            headerValue = strtok(NULL, "\n");
            if (headerValue != NULL)
                this->_headers.insert(std::pair<std::string, std::string>(headerKey, headerValue));
        }
        strtok(NULL, "\n");
        // for (std::map<std::string, std::string>::iterator itr =this->_headers.begin(); itr != this->_headers.end(); ++itr)
		// 				std::cout << itr->first << ": " << itr->second << std::endl;
        this->_body = request;
        // std::cout << this->_body << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
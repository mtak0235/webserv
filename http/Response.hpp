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
    int getStatusCode(void) const;
    void setStatusCode(const int& n);
    void setStatusMsg(const std::string& str);
    void setServerName(const std::string& str);
    void setLocation(const std::string& str);
    void setContentType(std::string str);

    void setConnection(std::string str);
    void setContentLanguage(std::string str);
    void setContentLength(std::string str);
    void setContentEncoding(std::string str);
    void setContentRange(std::string str);
    void setContentDisposition(std::string str);
    void setAcceptEncoding(std::string str);
    void setAcceptLanguage(std::string str);
    void setLastModified(std::string str);
    void setTransferEncoding(std::string str);

  private:
    /* private static variable */
    static const std::string _httpVersion;

    /* private variable */
    int _statusCode;
    std::string _statusMsg;
    std::string _location;
    std::string _serverName;

		std::string _contentType;
    std::string _connection;
    std::string _contentLanguage;
    std::string _contentLength;
    std::string _contentEncoding;
    std::string _contentRange;
    std::string _contentDisposition;
    std::string _acceptEncoding;
    std::string _acceptLanguage;
    std::string _lastModified;
    std::string _transferEncoding;


    /* private functions */
    std::string _makeFormatedNum(const int& num);
    std::string _getDate(void);
    std::string _makeStatusLine(void);
    std::string _makeHeader(void);
    std::string _makeLocation(void);
    std::string _makeServerInfo(void);

    std::string _makeContentType(void);
    std::string _makeConnection(void);
    std::string _makeContentLanguage(void);
    std::string _makeContentLength(void);
    std::string _makeContentEncoding(void);
    std::string _makeContentRange(void);
    std::string _makeContentDisposition(void);
    std::string _makeAcceptEncoding(void);
    std::string _makeAcceptLanguage(void);
    std::string _makeLastModified(void);
    std::string _makeTransferEncoding(void);
};
#endif

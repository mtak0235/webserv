#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct FileInfo
{
  std::string boundaryCode;
  std::string fileName;
  std::string data;
  std::string type;
};

class Request
{
  public:
  /* enumerator */
    enum HEADER_INFO
    {
      HOST,
      USER_AGENT,
      ACCEPT,
      ACCEPT_LANGUEAGE,
      ACCEPT_ENCODING,
      ACCEPT_CHARSET,
      KEEPALIVE,
      CONNECTION,
      CONTENT_TYPE,
      CONTENTS_LENGTH,
      AVAIL_H
    };

    enum METHOD
    {
      GET,
      POST,
      DELETE,
      AVAIL_M
    };

    /* constructor & destructor */
    Request();
    Request(const std::string& r);
    ~Request();

    /* public function */
    void clear(void);

    /* getter & setter */
    void setRequest(const std::string& r);
    std::string getMethod(void) const;
    std::string getPath(void) const;
    std::string getHttpVersion(void) const;
    std::string getHeaderInfo(const HEADER_INFO& i) const;
    std::string getBody(void) const;
    std::vector<FileInfo> getFileInfo(void) const;

  private:
    /* private static variable */
    static const std::string _availMethods[AVAIL_M];
    static const std::string _availHeaderInfos[AVAIL_H];

    /* private variable */
    std::string _method;
    std::string _path;
    std::string _httpVersion;
    std::string _headerInfo[AVAIL_H];
    std::string _rawBody;
    std::vector<FileInfo> _fileInfo;

    /* private functions */
    void _init(const std::string& r);
    void _parseRequestLine(const std::string& rl);
    void _parseRequestHeader(const std::string& rh);
    void _parseFileInfo(void);
};

#endif

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <iostream>


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
    enum HEADER_INFO {
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

	public:
		Request();
		Request(const std::string& r);
		~Request();

		void setRequest(const std::string& r);
		std::string getMethod(void) const;
		std::string getPath(void) const;
		std::string getHttpVersion(void) const;
		std::string getHeaderInfo(const HEADER_INFO& i) const;
    std::string getBody() const;
    std::vector<FileInfo> getFileInfo(void) const;
		void clear();

  private:
    enum METHOD {
    GET,
    POST,
    DELETE,
    AVAIL_M
    };

    static const std::string _availMethods[AVAIL_M];
    static const std::string _availHeaderInfos[AVAIL_H];

    void _init(const std::string& r);
    void _parseRequestLine(const std::string& rl);
    void _parseRequestHeader(const std::string& rh);
    void _parseFileInfo(void);
    // void _parseRequestBody(const std::string& rb);

    /* request line */
    std::string _method;
    std::string _path;
    std::string _httpVersion;

    /* request header */
    std::string _headerInfo[AVAIL_H];

    /* request body */
    std::string _rawBody;

    /* */
    std::vector<FileInfo> _fileInfo;
};

#endif

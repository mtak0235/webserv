#pragma once
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

class Request
{
    enum METHOD {
        GET,
        POST,
        DELETE,
        AVAIL_M,
    };

    enum HEADER_INFO {
        HOST,
        USER_AGENT,
        ACCEPT,
        ACCEPT_LANGUEAGE,
        ACCEPT_ENCODING,
        ACCEPT_CHARSET,
        KEEPALIVE,
        CONNECTION,
        CONTENTS_LENGTH,
        AVAIL_H,
    };

    public:
        Request(const std::string& r);
        ~Request();

        std::string getMethod(void) const;
        std::string getPath(void) const;
        std::string getHttpVersion(void) const;

    private:

        static const std::string _availMethods[AVAIL_M];
        static const std::string _availHeaderInfo[AVAIL_H];

        void _init(const std::string& r);
        void _parseRequestLine(const std::string& rl);
        void _parseRequestHeader(const std::string& rh);


        /* request line */
        std::string _method;
        std::string _path;
        std::string _httpVersion;

        /* request header */
        std::string _headerInfo[AVAIL_H];
};

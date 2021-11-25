#include "Request.hpp"

const std::string Request::_availMethods[AVAIL_M] = {"GET", "POST", "DELETE"};
const std::string Request::_availHeaderInfo[AVAIL_H] = {"Host:", "User-Agent:", "Accept:", "Accept-Language:", "Accept-Encoding:", "Accept-Charset:", "Keep-Alive:", "Connection:", "Content-Length:"};

Request::Request(const std::string& r) {
  _init(r);
}

Request::~Request()
{
}

std::string Request::getMethod(void) const
{
	return _method;
}

std::string Request::getPath(void) const
{
	return _path;
}

std::string Request::getHttpVersion(void) const
{
	return _httpVersion;
}

void Request::_init(const std::string& r) {
    std::stringstream ss;
    ss << r;
    std::vector<std::string> v;
    while (!ss.eof()) {
        std::string l;
        std::getline(ss, l);
        v.push_back(l);
    }
    _parseRequestLine(v[0]);
    for (size_t l = 1; l < v.size(); l++) {
        _parseRequestHeader(v[l]);
        if (v[l][0] == '\r') {
            _parseRequestBody(v[++l]);
            break ;
        }
    }

    /* test */
    std::cout << "test method : ["  << _method << "]\n";
    std::cout << "test path : ["  << _path << "]\n";
    std::cout << "test version : ["  << _httpVersion << "]\n";

    /* test */
    for (int h = 0; h < AVAIL_H; h++) {
        std::cout << _availHeaderInfo[h] << " : ["  << _headerInfo[h] << "]\n";
    }

    /* test */
    std::cout << "test raw body : [" << _rawBody << "]\n";

    std::map<std::string, std::string>::iterator i = _body.begin();
    while (i != _body.end()) {
        std::cout << i->second << "\n";
        i++;
    }
}

void Request::_parseRequestLine(const std::string& rl) {
    size_t idx = 0;
    for (int m = 0; m < AVAIL_M; m++) {
        if (rl.find(_availMethods[m], idx) == 0) {
            _method = _availMethods[m];
            idx += _availMethods[m].length() + 1;
            break ;
        }
    }
    _path = rl.substr(idx, rl.find(' ', idx + 1) - idx);
    idx += _path.length() + 1;
    _httpVersion = rl.substr(idx, rl.find('\n', idx + 1) - idx);
}

void Request::_parseRequestHeader(const std::string& rh) {
    size_t idx = 0;
    for (int h = 0; h < AVAIL_H; h++) {
        if (rh.find(_availHeaderInfo[h], idx) == 0) {
            idx += _availHeaderInfo[h].length() + 1;
            _headerInfo[h] = rh.substr(idx);
        }
    }
}


void Request::_parseRequestBody (const std::string& rb) {
    _rawBody = rb;
    for (size_t i = 0; i < _rawBody.size(); i++) {
        if (_rawBody[i] == '+') _rawBody[i] = ' ';
    }
    size_t idx = 0;
    while (idx < _rawBody.size()) {
        size_t idxSep;
        if ((idxSep = _rawBody.find('=', idx + 1)) == std::string::npos)
            break ;
        std::string key = _rawBody.substr(idx, idxSep - idx);
        idx += key.size() + 1;
        if ((idxSep = _rawBody.find('&', idx + 1)) == std::string::npos) {
            _body[key] = _rawBody.substr(idx);
            break ;
        } else {
            _body[key] = _rawBody.substr(idx, idxSep - idx);
            idx = idxSep + 1;
        }
    }
}

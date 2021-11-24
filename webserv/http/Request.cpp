#include "Request.hpp"

const std::string Request::_availMethods[AVAIL_M] = {"GET", "POST", "DELETE"};
const std::string Request::_availHeaderInfo[AVAIL_H] = {"Host:", "User-Agent:", "Accept:", "Accept-Language:", "Accept-Encoding:", "Accept-Charset:", "Keep-Alive:", "Connection:", "Content-Length:"};

Request::Request()
{

}

Request::Request(const std::string& r) 
{
  _init(r);
}

Request::~Request()
{
}

void Request::setRequest(const std::string& r)
{
	_init(r);
}

void Request::clear()
{
	/* request line */
	_method = "";
	_path = "";
	_httpVersion = "";
	/* request header */
	for (int i = 0; i < AVAIL_H; i++)
		_headerInfo[i] = "";
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
    for (size_t l = 1; l < v.size(); l++)
        _parseRequestHeader(v[l]);

    // /* test */
    // std::cout << "test method : ["  << _method << "]\n";
    // std::cout << "test path : ["  << _path << "]\n";
    // std::cout << "test version : ["  << _httpVersion << "]\n";
    
    // /* test */
    // for (int h = 0; h < AVAIL_H; h++) {
    //     std::cout << _availHeaderInfo[h] << " : ["  << _headerInfo[h] << "]\n";
    // }
}

void Request::_parseRequestLine(const std::string& rl) {
    size_t idx = 0;
    for (int m = 0; m < AVAIL_M; m++) {
        if (rl.find(_availMethods[m], idx) == 0) {
            _method = _availMethods[m];
            idx += _availMethods[m].length() + 1;
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

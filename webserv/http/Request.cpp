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

<<<<<<< HEAD
void Request::_init(const std::string& r) {
=======
void Request::_init(const std::string& r) {//post mapping 시 바디 부분 파싱 내용이 없다. get 방식 떄도 개행을 붙여서 리턴을 할까?
>>>>>>> cgi
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
<<<<<<< HEAD
=======
//    4
// post /he http/1.1
// 0123456789(idx)
// idx = 5;/ exists;
// idx = 9;
>>>>>>> cgi

void Request::_parseRequestLine(const std::string& rl) {
    size_t idx = 0;
    for (int m = 0; m < AVAIL_M; m++) {
        if (rl.find(_availMethods[m], idx) == 0) {
            _method = _availMethods[m];
            idx += _availMethods[m].length() + 1;
<<<<<<< HEAD
=======
            break;
>>>>>>> cgi
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

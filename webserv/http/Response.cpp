#include "Response.hpp"
#include <cstring>

const std::string Response::_httpVersion = "1.1";

Response::Response(void)
    : _statusCode(0), _statusMsg("") /*, _contentLength(0)*/
{
}

Response::~Response()
{
}

std::string Response::makeResponse(const std::string& body) {
  std::string ret = "";
  ret += (_makeStatusLine() + "\n");
  ret += (_makeHeader() + "\n");
  ret += body;
  return ret;
}

void Response::setStatusCode(const int& n) {
  _statusCode = n;
}

void Response::setStatusMsg(const std::string& str) {
  _statusMsg = str;
}

void Response::setServerName(const std::string& str) {
  _serverName = str;
}

std::string Response::_makeServerInfo(void) {
  std::string ret = "Server: ";
  ret += _serverName + "\n";
  return ret;
}

std::string Response::_makeStatusLine() {
  std::stringstream ss;
  ss << "HTTP/" << _httpVersion + " " << _statusCode << " " << _statusMsg << "\n";
  std::string ret;
  std::getline(ss, ret);
  ret + "\n";
  return ret;
}

std::string Response::_makeHeader() {
  std::string ret;
  ret += ("Date: " + Formatter::getDate());
  ret += _makeServerInfo();
  return ret;
}

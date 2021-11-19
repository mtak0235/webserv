#include "Response.hpp"

Response::Response(void)
{
}

Response::~Response()
{
}

std::string Response::makeResponse() {
  std::string ret = "";
  ret += _makeStatusLine();
  ret += _makeHeader();
  ret += _makeBody();
  return ret;
}

void Response::setHttpVersion(const std::string& n) {
  _httpVersion = n;
}

void Response::setStatusCode(const int& n) {
  _statusCode = n;
}

void Response::setStatusMsg(const std::string& str) {
  _statusMsg = str;
}

std::string Response::_makeStatusLine() {
  std::stringstream ss;
  ss << "HTTP/ " << _httpVersion + " " << _statusCode << " " << _statusMsg << "\n";
  std::string ret;
  std::getline(ss, ret);
  return ret;
}

std::string Response::_makeHeader() {
  return "";
}

std::string Response::_makeBody() {
  return "";
}

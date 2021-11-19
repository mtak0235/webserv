#include "Response.hpp"

const std::string Response::_httpVersion = "1.1";

Response::Response(void)
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

std::string Response::_makeStatusLine() {
  std::stringstream ss;
  ss << "HTTP/" << _httpVersion + " " << _statusCode << " " << _statusMsg << "\n";
  std::string ret;
  std::getline(ss, ret);
  return ret;
}

std::string Response::_makeHeader() {

  return "";
}

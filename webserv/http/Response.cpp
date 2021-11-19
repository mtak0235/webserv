#include "Response.hpp"

const std::string Response::_httpVersion = "1.1";

Response::Response(void)
    : _statusCode(0), _statusMsg(""), _contentLength(0)
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

std::string Response::_makeStatusLine() {
  std::stringstream ss;
  ss << "HTTP/" << _httpVersion + " " << _statusCode << " " << _statusMsg << "\n";
  std::string ret;
  std::getline(ss, ret);
  return ret;
}

std::string Response::_makeHeader() {
  std::string ret = "Date: ";
  time_t timer = time(NULL);
  struct tm* t = localtime(&timer);
  const std::string wday[7] = {"Sun", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat"};
  ret += (wday[t->tm_wday] + ", ");
  ret += (_getFormattedNum(t->tm_mday) + " ");
  const std::string wmon[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  ret += (wmon[t->tm_mon] + " ");

  return ret;
}

std::string Response::_getFormattedNum(const int& num) {
  std::string ret = "";
  if (num < 10) ret += "0";
  std::stringstream ss;
  ss << num;
  std::string str;
  ss >> str;
  ret += str;
  return ret;
}

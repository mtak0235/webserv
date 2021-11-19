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

std::string Response::_makeStatusLine() {

}

std::string Response::_makeHeader() {

}

std::string Response::_makeBody() {

}

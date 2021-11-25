#include "Cgi.hpp"

const std::string Cgi::_environList[NON_OF_ALL] = {"AUTH_TYPE", "CONTENT_LENGTH", "CONTENT_TYPE", "GATEWAY_INTERPACE", "PATH_INFO",
                                                   "PATH_TRANSLATED", "QUERY_STRING", "REMOTE_ADDR", "REMOTE_IDENT", "REMOTE_USER",
                                                   "REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME", "SERVER_NAME", "SERVER_PORT",
                                                   "SERVER_PROTOCOL", "SERVER_SOFTWARE", "REDIRECT_STATUS"};

Cgi::Cgi(void)
{

}

Cgi::~Cgi(void)
{
  if (_environ) delete[] _environ;
}

std::string Cgi::getCgiResponse(Request req)
{
  _setEnviron(req);

}

void Cgi::_setEnviron(const Request& req) {
  std::map<std::string, std::string> envMap = _makeEnvMap(req);
  size_t allocSize = envMap.size() + 1;
  _environ = new char*[allocSize];
  if (!_environ) return ;
  size_t i = 0;


}

const std::string Cgi::_getCwd(const std::string& path) const {
  const int buffSize = 512;
  char buff[buffSize];
  getcwd(buff, buffSize);
  std::string ret = buff;
  return ret;
}

std::map<std::string, std::string> Cgi::_makeEnvMap(const Request& req) const {
  std::map<std::string, std::string> ret;
  ret[_environList[REQUEST_METHOD]] = req.getMethod();
  ret[_environList[SCRIPT_FILENAME]] = req.getPath();
  ret[_environList[PATH_INFO]] = req.getPath();
  ret[_environList[REQUEST_URI]] = req.getPath();
  ret[_environList[REDIRECT_STATUS]] = "CGI";
  ret[_environList[SERVER_PROTOCOL]] = "HTTP/1.1";
  ret[_environList[CONTENT_TYPE]] = "application/x-www-form-urlencoded";
  ret[_environList[GATEWAY_INTERFACE]] = "CGI/1.1";
  ret[_environList[REMOTE_ADDR]] = "127.0.0.1";
  ret[_environList[SERVER_PORT]] = "80";
  ret[_environList[SERVER_SOFTWARE]] = "versbew";
  ret[_environList[PATH_TRANSLATED]] = _getCwd(req.getPath());
  return ret;
}

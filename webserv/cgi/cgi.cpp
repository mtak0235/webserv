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
  
}

std::string Cgi::getCgiResponse(Request req)
{
  _setEnviron(req);

}

void Cgi::_setEnviron (const Request& req) {
  




	  _env = _setEnviron(env_set);

}

const std::string Cgi::_getCwd (const std::string& path) const {
  const int buffSize = 512;
  char buff[buffSize];
  getcwd(buff, buffSize);
  std::string ret = buff;
  return ret;
}

size_t Cgi::_getEnvironListSize (const Request& req) {
  std::map<std::string, std::string> envMap;
  envMap[_environList[REQUEST_METHOD]] = req.getMethod();
  envMap[_environList[SCRIPT_FILENAME]] = req.getPath();
  envMap[_environList[PATH_INFO]] = req.getPath();
  envMap[_environList[REQUEST_URI]] = req.getPath();
  envMap[_environList[REDIRECT_STATUS]] = "CGI";
  envMap[_environList[SERVER_PROTOCOL]] = "HTTP/1.1";
  envMap[_environList[CONTENT_TYPE]] = "application/x-www-form-urlencoded";
  envMap[_environList[GATEWAY_INTERFACE]] = "CGI/1.1";
  envMap[_environList[REMOTE_ADDR]] = "127.0.0.1";
  envMap[_environList[SERVER_PORT]] = "80";
  envMap[_environList[SERVER_SOFTWARE]] = "versbew";
  envMap[_environList[PATH_TRANSLATED]] = _getCwd(req.getPath());
  return envMap.size();
}

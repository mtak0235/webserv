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
  for (size_t i = 0; i < _allocSize; i++) {
    if (_environ[i]) delete[] _environ[i];
  }
  if (_environ) delete[] _environ;
}

std::string Cgi::getCgiResponse(Request req)
{
  std::string ret = "";
  _setEnviron(req);
  FILE *fIn = tmpfile();
  FILE *fOut = tmpfile();
  long fdIn = fileno(fIn);
  long fdOut = fileno(fOut);
  std::string testInput = "hElloWorld";
  write(fdIn, testInput.c_str(), testInput.size());
  lseek(fdIn, 0, SEEK_SET);
  pid_t pid = fork();
  if (pid == 0) {
    dup2(fdIn, STDIN_FILENO);
    dup2(fdOut, STDOUT_FILENO);
    execve(req.getPath().c_str(), NULL, _environ);
  } else {
    const size_t buffSize = 5;
    char buff[buffSize] = {0, };
    wait(NULL);
    lseek(fdOut, 0, SEEK_SET);

    size_t bytes = 0;
    while ((bytes = read(fdOut, buff, buffSize)) != 0) {
      ret += buff;
      memset(buff, 0, buffSize);
    }
  }
  return ret;
}

void Cgi::_setEnviron(const Request& req) {
  std::map<std::string, std::string> envMap = _makeEnvMap(req);
  _allocSize = envMap.size() + 1;
  _environ = new char*[_allocSize];
  if (!_environ) return ;
  std::map<std::string, std::string>::iterator itMap = envMap.begin();
  size_t idxEnv = 0;
  while (itMap != envMap.end()) {
    std::string temp = itMap->first + "=" + itMap->second;
    _environ[idxEnv] = new char[temp.size() + 1];
    _environ[idxEnv] = (char*)temp.c_str();
    idxEnv++;
    itMap++;
  }
  _environ[idxEnv] = NULL;
}

const std::string Cgi::_getCwd(void) const {
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
  ret[_environList[PATH_TRANSLATED]] = _getCwd();
  return ret;
}

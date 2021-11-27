#include "Cgi.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


const std::string Cgi::_environList[NON_OF_ALL] = {"AUTH_TYPE", "CONTENT_LENGTH", "CONTENT_TYPE", "GATEWAY_INTERPACE", "PATH_INFO",
                                                   "PATH_TRANSLATED", "QUERY_STRING", "REMOTE_ADDR", "REMOTE_IDENT", "REMOTE_USER",
                                                   "REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME", "SERVER_NAME", "SERVER_PORT",
                                                   "SERVER_PROTOCOL", "SERVER_SOFTWARE", "REDIRECT_STATUS"};

Cgi::Cgi(void)
{

}

Cgi::~Cgi(void)
{
  // for (size_t i = 0; i < _allocSize; i++) {
    // if (_environ[i]) delete[] _environ[i];
  // }
  // if (_environ) delete[] _environ;
}

std::string Cgi::getCgiResponse(Request req)
{
  std::string ret = "";
  _setEnviron(req);

  char c[15] = "./cgi_tester";
  char *argv[] = {c, NULL};
  // char *envp[] = {"REQUEST_METHOD=GET", "SERVER_PROTOCOL=HTTP/1.1", "PATH_INFO=/i", NULL};

  pid_t pid;

  FILE *fIn = tmpfile();
  FILE *fOut = tmpfile();
  long fdIn = fileno(fIn);
  long fdOut = fileno(fOut);

  std::string testInput = "hElloWorld";
  write(fdIn, testInput.c_str(), testInput.size());
  lseek(fdIn, 0, SEEK_SET);
  
  pid = fork();
  if (pid == 0) 
  {
    dup2(fdIn, STDIN_FILENO);
    dup2(fdOut, STDOUT_FILENO);
    execve("./cgi_tester", argv, _environ);
  } 
  else 
  {
    const size_t buffSize = 2048;
    char buff[buffSize] = {0, };
    waitpid(-1, NULL, 0);
    lseek(fdOut, 0, SEEK_SET);
    size_t bytes = 0;
    while ((bytes = read(fdOut, buff, 1)) != 0)
		{
			buff[bytes] = 0;
      std::string temp = buff;
      ret += temp;
			memset(buff, 0, sizeof(buff));
    }
  }
  std::cout << "test cgi [" << ret << "] \n";
  return ret;
}

void Cgi::_setEnviron(const Request& req) {
  std::map<std::string, std::string> envMap = _makeEnvMap(req);
  _allocSize = envMap.size() + 1;
	_environ = new char*[_allocSize];
  std::map<std::string, std::string>::iterator itMap = envMap.begin();
  size_t idxEnv = 0;
  while (itMap != envMap.end()) {
    std::string temp = itMap->first + "=" + itMap->second;
		_environ[idxEnv] = new char[temp.size() + 1];
		for (unsigned long i = 0; i < temp.size(); i++)
    	_environ[idxEnv][i] = temp[i];
		_environ[idxEnv][temp.size()] = 0;
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

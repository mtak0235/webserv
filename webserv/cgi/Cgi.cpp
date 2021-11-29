#include "Cgi.hpp"
#include <fcntl.h>

const std::string Cgi::_environList[NON_OF_ALL] = {"AUTH_TYPE", "CONTENT_LENGTH", "CONTENT_TYPE", "GATEWAY_INTERPACE", "PATH_INFO",
                                                   "PATH_TRANSLATED", "QUERY_STRING", "REMOTE_ADDR", "REMOTE_IDENT", "REMOTE_USER",
                                                   "REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME", "SERVER_NAME", "SERVER_PORT",
                                                   "SERVER_PROTOCOL", "SERVER_SOFTWARE", "REDIRECT_STATUS"};

Cgi::Cgi(void)
{
  _statusCode = 400;
}

Cgi::~Cgi(void)
{
}

std::string Cgi::_getInput(std::string file)
{
  std::string body = "";
  char c;
  std::ifstream ifs;
  ifs.open(file);
  if (!ifs)
  {
    std::cout << "file open error\n";
    _statusCode = 404;
  }
  else
  {
    while (ifs.get(c))
      body += c;
    _statusCode = 200;
  }
  ifs.close();
  return body;
}

int Cgi::getStatusCode()
{
  return _statusCode;
}

void Cgi::_setCgiResponseHeader(const std::vector<std::string>& v)
{
	if (v.size() >= 2)
		_cgiResponseHeader = v[0] + '\n' + v[1];
}

void Cgi::_setCgiResponseBody(const std::vector<std::string>& v)
{
	std::string temp;
	for (size_t i = 2; i < v.size(); i++)
	{
		temp += v[i];
		if (i != v.size() - 1) temp += '\n';
	}
	// std::cout << '[' << result << "]\n";
	_cgiResponseBody = temp;
}

void Cgi::execute(Request req, std::string cgiFilePath, std::string file)
{
  // cgiFilePath = "." + cgiFilePath;
  std::string result = "";
	std::string cgiInput; // php req.getBody();
	if (cgiFilePath.find("cgi_tester") == std::string::npos)
	{
		_statusCode = 200;
		cgiInput = req.getBody();
	}
	else
		cgiInput = _getInput(file);
  std::string a = cgiFilePath;
  _setEnviron(req, file);
	// std::cout << "file [" << file << "]\n";
  // std::cout << "cgi File path [" << cgiFilePath << "]\n";
  // std::cout << "cgi input [" << cgiInput << "]\n";

  char *tmp = new char[a.size() + 1];
  for (unsigned long i = 0; i < a.size(); i++)
    tmp[i] = a[i];
  tmp[a.size()] = 0;
  char *argv[] = {tmp, NULL};
  std::cout << "[" << argv[0] << "]" << std::endl;
  FILE *fIn = tmpfile();
  FILE *fOut = tmpfile();
  long fdIn = fileno(fIn);
  long fdOut = fileno(fOut);

  write(fdIn, cgiInput.c_str(), cgiInput.size());
  lseek(fdIn, 0, SEEK_SET);

  pid_t pid = fork();
  if (pid == 0)
  {
    dup2(fdIn, STDIN_FILENO);
    dup2(fdOut, STDOUT_FILENO);
    execve(cgiFilePath.c_str(), argv, _environ);
  }
  else
  {
    const size_t buffSize = 1048;
    char buff[buffSize] = {
        0,
    };
    wait(NULL);
    lseek(fdOut, 0, SEEK_SET);
    size_t bytes = 0;
    while ((bytes = read(fdOut, buff, 1)) != 0)
    {
      std::string temp = buff;
      result += temp;
    }
  }
	std::vector<std::string> v;
  std::stringstream ss;
  ss << result;
  while (!ss.eof())
  {
    std::string l;
    std::getline(ss, l);
    v.push_back(l);
  }
	_setCgiResponseHeader(v);
	_setCgiResponseBody(v);
	delete[] tmp;
}

void Cgi::_setEnviron(const Request &req, std::string file)
{
  std::map<std::string, std::string> envMap = _makeEnvMap(req, file);
  _allocSize = envMap.size() + 1;
  // _environ = (char**)malloc(sizeof(char *) * _allocSize);
  _environ = new char *[_allocSize];
  std::map<std::string, std::string>::iterator itMap = envMap.begin();
  size_t idxEnv = 0;
  while (itMap != envMap.end())
  {
    std::string temp = itMap->first + "=" + itMap->second;
    // _environ[idxEnv] = (char*)malloc(temp.size() + 1);
    // _environ[idxEnv] = (char*)temp.c_str();
    _environ[idxEnv] = new char[temp.size() + 1];
    for (unsigned long i = 0; i < temp.size(); i++)
      _environ[idxEnv][i] = temp[i];
    _environ[idxEnv][temp.size()] = 0;
    printf("%s\n", _environ[idxEnv]);
    idxEnv++;
    itMap++;
  }
  _environ[idxEnv] = NULL;
}

const std::string Cgi::_getCwd(void) const
{
  const int buffSize = 512;
  char buff[buffSize];
  getcwd(buff, buffSize);
  std::string ret = buff;
  return ret;
}

std::map<std::string, std::string> Cgi::_makeEnvMap(const Request &req, std::string file) const
{
  std::map<std::string, std::string> ret;
  ret[_environList[REQUEST_METHOD]] = req.getMethod();
  ret[_environList[SCRIPT_FILENAME]] = file; ////서버 상 .php파일의 절대 위치
  ret[_environList[PATH_INFO]] = req.getPath();
  ret[_environList[REQUEST_URI]] = req.getPath();
  ret[_environList[REDIRECT_STATUS]] = "CGI";
  ret[_environList[SERVER_PROTOCOL]] = "HTTP/1.1";
  ret[_environList[CONTENT_TYPE]] = "application/x-www-form-urlencoded";
  ret[_environList[GATEWAY_INTERFACE]] = "CGI/1.1";
  ret[_environList[REMOTE_ADDR]] = "127.0.0.1";
  ret[_environList[SERVER_PORT]] = "80";
  ret[_environList[SERVER_SOFTWARE]] = "versbew";
  ret[_environList[PATH_TRANSLATED]] = req.getPath();
  ret[_environList[CONTENT_LENGTH]] = std::to_string(req.getBody().size());
  return ret;
}

std::string Cgi::getCgiResponseBody()
{
  return _cgiResponseBody;
}

std::string Cgi::getCgiResponseHeader()
{
  return _cgiResponseHeader;
}
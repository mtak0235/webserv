#include "Cgi.hpp"

Cgi::~Cgi()
{

}

char ** Cgi::_setEnviron(std::map<std::string, std::string> env) {
  char **return_value;
  std::string temp;

  return_value = (char **)malloc(sizeof(char *) * (env.size() + 1));
  int i = 0;
  std::map<std::string, std::string>::iterator it;
  for (it = env.begin(); it != env.end(); it++) {
    temp = (*it).first + "=" + (*it).second;
    char *p = (char *)malloc(temp.size() + 1);
    strcpy(p, temp.c_str());
    return_value[i] = p;
    i++;
  }
  return_value[i] = NULL;
  return (return_value);
}

std::string Cgi::execute(char  **_env, std::string value)
{
	std::string ret;
	pid_t pid;

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);

	write(fdIn, value.c_str(), sizeof(value.c_str()));
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();
	if (pid == 0)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve("./cgi_tester", NULL, _env);
	}
	else
	{
		char buf[1024] = {0,};
		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		int read_len = read(fdOut, buf, 1024);
		buf[read_len] = 0;
		ret = buf;
	}
	return ret;
}

char **Cgi::getEnv()
{
	return _env;
}
std::string setPathTranslated(const char *path) {
  std::string result;

  char current_dir[512];
  getcwd(current_dir, 512);
  result += current_dir;
  result += *(path + 1);
  return (result);
}

void Cgi::_setEnv(const Request *req)
{
	std::map<std::string, std::string> env_set;
    for (int i = 0; basic_env[i] != NULL; i++) {
      std::pair<std::string, std::string> env_temp;
      env_temp.first = basic_env[i];
      env_temp.second = "";
      env_set.insert(env_temp);
    }
	  env_set["REQUEST_METHOD"] = req->getMethod();
      env_set["REDIRECT_STATUS"] = "CGI";
      env_set["SCRIPT_FILENAME"] = std::string(req->getPath());
      env_set["SERVER_PROTOCOL"] = "HTTP/1.1";
      env_set["PATH_INFO"] = req->getPath();
      env_set["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
      env_set["GATEWAY_INTERFACE"] = "CGI/1.1";
      env_set["PATH_TRANSLATED"] = setPathTranslated(req->getPath().c_str());
      env_set["REMOTE_ADDR"] = "127.0.0.1";
      env_set["REQUEST_URI"] = req->getPath();
      env_set["SERVER_PORT"] = "80";
      env_set["SERVER_PROTOCOL"] = "HTTP/1.1";
      env_set["SERVER_SOFTWARE"] = "versbew";
	  _env = _setEnviron(env_set);
}

Cgi::Cgi(const Request *req)
{
	_setEnv(req);
}
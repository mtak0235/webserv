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
  // for (size_t i = 0; i < _allocSize; i++) {
    // if (_environ[i]) delete[] _environ[i];
  // }
  // if (_environ) delete[] _environ;
}

std::string Cgi::getCgiResponse(Request req, std::string filePath)
{
  std::string ret = "";
  std::string cgiInput = req.getBody();
  _setEnviron(req);
  FILE *fIn = tmpfile();
  FILE *fOut = tmpfile();
  long fdIn = fileno(fIn);
  long fdOut = fileno(fOut);
  write(fdIn, cgiInput.c_str(), cgiInput.size());
  lseek(fdIn, 0, SEEK_SET);
  pid_t pid = fork();
  if (pid == 0) {
    dup2(fdIn, STDIN_FILENO);
    dup2(fdOut, STDOUT_FILENO);
    execve(filePath.c_str(), NULL, _environ);
  } else {
    const size_t buffSize = 2048;
    char buff[buffSize] = {0, };
    wait(NULL);
    lseek(fdOut, 0, SEEK_SET);
    size_t bytes = 0;
    while ((bytes = read(fdOut, buff, 1)) != 0) {
      std::string temp = buff;
      ret += temp;
    }
  }
  std::cout << "test cgi [" << ret << "] \n";
  return ret;
}
/* 넘기는 데이터가 없는 cgi get 요청
GET /test_cgi/index.php HTTP/1.1
Host: localhost:8081
Connection: keep-alive
sec-ch-ua: "Google Chrome";v="89", "Chromium";v="89", ";Not A Brand";v="99"
sec-ch-ua-mobile: ?0
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*\/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Referer: http://localhost:8081/test_cgi/
Accept-Encoding: gzip, deflate, br
Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7
Cookie: SL_G_WPT_TO=en; SL_GWPT_Show_Hide_tmp=undefined; SL_wptGlobTipTmp=undefined

*/

/*넘기는 데이터가 있는  cgi post 요청
POST /test_cgi/post/action.php HTTP/1.1
Host: localhost:8081
Connection: keep-alive
Content-Length: 20
Cache-Control: max-age=0
sec-ch-ua: "Google Chrome";v="89", "Chromium";v="89", ";Not A Brand";v="99"
sec-ch-ua-mobile: ?0
Upgrade-Insecure-Requests: 1
Origin: http://localhost:8081
Content-Type: application/x-www-form-urlencoded
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*\/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Referer: http://localhost:8081/test_cgi/post/form.html
Accept-Encoding: gzip, deflate, br
Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7
Cookie: SL_G_WPT_TO=en; SL_GWPT_Show_Hide_tmp=undefined; SL_wptGlobTipTmp=undefined

name=glad&email=mtak
*/

void Cgi::_setEnviron(const Request& req) {
  std::map<std::string, std::string> envMap = _makeEnvMap(req);
  _allocSize = envMap.size() + 1;
  _environ = (char**)malloc(sizeof(char *) * _allocSize);
  std::map<std::string, std::string>::iterator itMap = envMap.begin();
  size_t idxEnv = 0;
  while (itMap != envMap.end()) {
    std::string temp = itMap->first + "=" + itMap->second;
    _environ[idxEnv] = (char*)malloc(temp.size() + 1);
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
  ret[_environList[SCRIPT_FILENAME]] = "./www"+ req.getPath();////서버 상 .php파일의 절대 위치
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

#pragma once
#include <map>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h> //gethostname()
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h> //gethostbyname()
#include <cstring> //memcpy(), strcpy()
#include <fstream>
#include <fcntl.h>
#include <map>
#include <vector>
#include <sys/wait.h>
#include <stdlib.h>

#include "../http/Request.hpp"

class Request;

static const char *basic_env[] = {
    "AUTH_TYPE",
    "CONTENT_LENGTH",
    "CONTENT_TYPE",
    "GATEWAY_INTERFACE",
    "PATH_INFO",
    "PATH_TRANSLATED",
    "QUERY_STRING",
    "REMOTE_ADDR",
    "REMOTE_IDENT",
    "REMOTE_USER",
    "REQUEST_METHOD",
    "REQUEST_URI",
    "SCRIPT_NAME",
    "SERVER_NAME",
    "SERVER_PORT",
    "SERVER_PROTOCOL",
    "SERVER_SOFTWARE",
    "REDIRECT_STATUS",
    NULL};

class Cgi
{
	private:
		Cgi(const Cgi &);
		Cgi& 				operator=(const Cgi &);
		char **_env;
		void _setEnv(const Request *);
		std::string _setPathTranslated(char *);
		char **_setEnviron(std::map<std::string, std::string>);
	
	public:
		Cgi(const Request *);
		~Cgi();
		char **getEnv();
		std::string				execute(char **, std::string);

	
};

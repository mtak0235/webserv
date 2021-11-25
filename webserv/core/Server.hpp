#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "ngxKqueue.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"

class Server : public ngxKqueue
{
	public :
		Server();
		~Server();
		void acceptNewClient(int servSock);
		int recvDataFromClient(int k);
		void setStatus();
		
	private :
		Log _log;
		Request _request;
		Response _response;
	
		char _buf[1024];
		int _readDataSize;
		std::string _clientReq;
		std::string _body;
		std::string _lastRespnse;
		int _statusCode;
		int _n;
		char _c;
		std::ifstream _ifs;
		std::vector<std::string> _indexList;
		
		std::vector<std::string> _allowMethods;
		std::string _requestMethod;
		bool _isAllow;
		LocationConfig _nowLocation;
		std::string _requestPath;

		size_t _found;
		std::string _isFile;

		int _responseDatatoServer(int k);
		void _getRequestInfo(int k);
		void _setResponse(int k);
		std::string _setBody(std::string file);
		std::string _getBody(std::string file, int k);

};

#endif

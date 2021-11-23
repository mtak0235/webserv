#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "ngxKqueue.hpp"
#include "Request.hpp"
#include "Response.hpp"

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
		char _c;
		std::ifstream _ifs;
		std::vector<std::string> _indexList;

		int _responseDatatoServer(int k);
		int _getRequestInfo();

};

#endif

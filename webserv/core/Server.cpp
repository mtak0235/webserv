#include "Server.hpp"

Server::Server()
{
	_readDataSize = 0;
	_clientReq = "";
	_statusCode = 503;
	_isAllow = false;
}

Server::~Server()
{

}

void Server::setStatus()
{
	_status[200] = "OK";
	// _status[201] = "Created";
	// _status[202] = "Accepted";
	// _status[203] = "Non-Authoritative Information";
	// _status[204] = "No Content";
	// _status[205] = "Reset Content";
	// _status[206] = "Partial Content";
	_status[300] = "Multiple Choice";
	// _status[301] = "Moved Permanently";
	// _status[302] = "Found";
	// _status[303] = "See Other";
	// _status[304] = "Not Modified";
	// _status[307] = "Temporary Redirect";
	// _status[308] = "Permanent Redirect";
	_status[400] = "Bad Request";
	// _status[401] = "Unauthorized";
	// _status[402] = "Payment Required";
	// _status[403] = "Forbidden";
	// _status[404] = "Not Found";
	_status[405] = "Method Not Allowed";
	// _status[406] = "Not Acceptable";
	// _status[407] = "Proxy Authentication Required";
	// _status[408] = "Request Timeout";
	_status[500] = "Internal Server Error";
	// _status[501] = "Not Implemented";
	// _status[502] = "Bad Gateway";
	// _status[503] = "Service Unavailable";
	// _status[504] = "Gateway Timeout";
	// _status[505] = "HTTP Version Not Supported";

}

void Server::acceptNewClient(int servSock)
{
	int client_socket;
	if ((client_socket = accept(servSock, NULL, NULL)) == -1)
		_log.debugLog("accept Error");
	// std::cout << "accept new client: " << client_socket << std::endl;
	fcntl(client_socket, F_SETFL, O_NONBLOCK);

	/* add event for client socket - add read && write event */
	changeEvents(_changeList, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	changeEvents(_changeList, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	_clients[client_socket] = "";
}

int Server::recvDataFromClient(int k)
{
	_readDataSize = read(_currEvent->ident, _buf, sizeof(_buf));
	if (_readDataSize <= 0)
	{
			if (_readDataSize < 0)
					std::cerr << "client read error!" << std::endl;
			disconnectClient(_currEvent->ident, _clients);
	}
	else
	{
		if (_responseDatatoServer(k) == NGX_FAIL)
			return NGX_FAIL;
	}
	return NGX_OK;
}

int Server::_responseDatatoServer(int k)
{
	_buf[_readDataSize] = '\0';
	_clients[_currEvent->ident] += _buf;
	_clientReq = _clients[_currEvent->ident];
	std::cout << "\033[36mreceived data from " << _currEvent->ident << "\033[37m\n" << _clientReq << std::endl;
	if (_clientReq != "")
	{
		_getRequestInfo(k);
		_setResponse(k);
		if ((_n = write(_currEvent->ident, _lastRespnse.c_str(), _lastRespnse.size()) == -1))
		{
			std::cerr << "client write error!" << std::endl;
			disconnectClient(_currEvent->ident, _clients);
		}
		else
			_clients[_currEvent->ident].clear();
		disconnectClient(_currEvent->ident, _clients);
		return NGX_OK;
	}
	return NGX_FAIL;
}


// allow method 안에서 있는지 확인
void Server::_getRequestInfo(int k)
{
	_request.clear();
	_request.setRequest(_clientReq);

	/* 테스트용 if 추가함 */
	if (!_request.getPath().compare("/cgi-tester"))
	{
		Cgi cgi;
		_requestMethod = "GET";
		_requestPath = _request.getPath();
		// _statusCode = cgi.
		_body = cgi.getCgiResponse(_request, _getCgiFilePath(_requestPath));
		std::cout << "\033[35mResponse Body\n" << _body << "\033[37m\n";
	}
	else
	{
		if (!_request.getPath().compare("/favicon.ico"))
			_request.setRequest(_request.getMethod() + " / " + _request.getHttpVersion());
		_requestPath = _request.getPath();
		_requestMethod = _request.getMethod();
		//파일인 경우
		_found = _requestPath.find_last_of(".");
		_isFile = _requestPath.substr(_found + 1);
		_body = "";
		if (!_isFile.compare("html") || !_isFile.compare("htm") || !_isFile.compare("bla"))
		{
			_body = _getBody(_requestPath.substr(1), k);
			return;
		}
		//경로인 경우
		_nowLocation = _serverConfigs[k].getLocationsFind(_requestPath);
		_allowMethods = _nowLocation.getAllowMethod();
		_isAllow = false;
		for (unsigned long i = 0; i < _allowMethods.size(); i++)
		{
			if (!_allowMethods[i].compare(_requestMethod))
				_isAllow = true;
		}
		if (!_isAllow)
			_statusCode = 405;
		else
		{
			_indexList = _nowLocation.getIndexList();
			_isFile = "";
			_found = 0;
			if (_indexList.size() > 0) //경로인 경우
			{
				_found = _indexList[0].find_last_of(".");
				_isFile = _indexList[0].substr(_found + 1);
				_body = _getBody(_indexList[0], k);
			}
		}
	}
	// iferrsetBody();
}

void Server::_setResponse(int k)
{
	_response.setServerName(_serverConfigs[k].getServerName());
	_response.setStatusCode(_statusCode);
	_response.setStatusMsg(_status[_statusCode]);
	_body += "\n";
	//+ content type
	_lastRespnse = _response.makeResponse(_body);
	std::cout << "\033[35msetResponse->_lastResopnse" << _lastRespnse << "\033[37m" << std::endl;
}

std::string Server::_setBody(std::string file)
{
	std::string body = "";
	_ifs.open(file);
	if (!_ifs)
	{
		_log.debugLog("file open error");
		_statusCode = 404;
	}
	else
	{
		while (_ifs.get(_c))
			body += _c;
		_statusCode = 200;
	}
	_ifs.close();
	return body;
}

std::string Server::_getCgiFilePath(std::string fileName)
{
	if (fileName.find("cgi_tester", 0) != std::string::npos)
		return "./www/bin/cgi_tester";
	else
		return "./www/bin/php-cgi";
}

std::string Server::_getBody(std::string file, int k)
{
	std::string body;

	std::cout << "\033[33m_getBody->file = " << file <<  "\033[37m\n";
	if (!_requestMethod.compare("GET"))
	{
		if (!_isFile.compare("html") || !_isFile.compare("htm"))
		{
			body = _setBody(file);
		}
		else
		{
			body = _cgi.getCgiResponse(this->_request, _getCgiFilePath(file));// 이거랑 
			_statusCode = 200;
		}
	}
	else if (!_requestMethod.compare("POST"))
		body = _cgi.getCgiResponse(this->_request, _getCgiFilePath(file));//같음
	else if (!_requestMethod.compare("DELETE"))
	{
		_serverConfigs[k].eraseLocation(_requestPath);
		body = "{\"success\":\"true\"}";
		_statusCode = 200;
	}
	return body;
}

//location block 탐색해서 그에 대한 정보를 지움 프로그램 안에서

// post 성공적으로 post 됬다는 메시지를 보냄

// get

// [ abcdef] [서브밋]/포스트
// post /save http/1.1
// cotent-type: text/json

// {acdef}
// fd = open(OCREATE)
// //
// acdef
// ab[delete] /delete
// //
// delete /remove HTTP/1.1
// content-Type : text/json

// {ab}
// //fd = ab(x)
// cdef

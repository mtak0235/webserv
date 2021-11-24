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
	std::cout << "accept new client: " << client_socket << std::endl;
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
	std::cout << "received data from " << _currEvent->ident << ": " << _clientReq << std::endl;
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
	if (!_request.getPath().compare("/favicon.ico"))
		_request.setRequest(_request.getMethod() + " / " + _request.getHttpVersion());
	_requestPath = _request.getPath();
	//파일인 경우
	size_t found = _requestPath.find_last_of(".");
	std::string isHtml = _requestPath.substr(found + 1);
	if (!isHtml.compare("html") || !isHtml.compare("htm") || !isHtml.compare("bla"))
	{
		_body = _setBody(_requestPath.substr(1));
		return;
	}
	//경로인 경우
	_nowLocation = _serverConfigs[k].getLocationsFind(_requestPath);
	_allowMethods  = _nowLocation.getAllowMethod();
	_requestMethod =  _request.getMethod();
	for (unsigned long i = 0; i < _allowMethods.size(); i++)
	{
		if (!_allowMethods[i].compare(_requestMethod))
			_isAllow = true;
	}
	if (!_isAllow)
		_statusCode = 405;
	else
	{
		if (!_requestMethod.compare("GET"))
		{
			_indexList = _nowLocation.getIndexList();
			if (_indexList.size() > 0) //경로인 경우
			{
				size_t found = _indexList[0].find_last_of(".");
				isHtml = _indexList[0].substr(found + 1);
				if (!isHtml.compare("html") || !isHtml.compare("htm"))
				{
					std::cout << _indexList[0] << "\n";
					_body = _setBody(_indexList[0]);
				}
				//else
				//cgi	
			}
		}
		else if (!_requestMethod.compare("POST"))
		{
			
		}
		else if (!_requestMethod.compare("DELETE"))
		{

		}
	}
	//iferrsetBody();
}

void Server::_setResponse(int k)
{
	_response.setServerName(_serverConfigs[k].getServerName());
	_response.setStatusCode(_statusCode);
	_response.setStatusMsg(_status[_statusCode]);
	_body += "\n";
	_lastRespnse = _response.makeResponse(_body);
	std::cout <<_lastRespnse;
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

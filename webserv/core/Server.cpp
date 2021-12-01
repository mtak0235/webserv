#include "Server.hpp"

Server::Server()
{
	_readDataSize = 0;
	_clientReq = "";
	_statusCode = 500;
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
	_status[302] = "Found";
	// _status[303] = "See Other";
	// _status[304] = "Not Modified";
	// _status[307] = "Temporary Redirect";
	// _status[308] = "Permanent Redirect";
	_status[400] = "Bad Request";
	// _status[401] = "Unauthorized";
	// _status[402] = "Payment Required";
	_status[403] = "Forbidden";
	_status[404] = "Not Found";
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
	std::cout << "\033[36m[RECEIVED DATA FROM " << _currEvent->ident << "]\033[37m\n" << _clientReq << std::endl;
	if (_clientReq != "")
	{
		_statusCode = 500;
		_setRequestInfo(k);
		_setResponse(k);
	std::cout << "\033[36m[RESPOND DATA" << "]\033[37m\n" << _lastRespnse << std::endl;
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

int Server::_fileJudge(int k)
{
	_found = _requestPath.find_last_of(".");
	_isFile = _requestPath.substr(_found + 1);
	int slashCnt = 0;
	for (unsigned long i = 0; i < _requestPath.size(); i++)
	{
		if (_requestPath[i] == '/')
			slashCnt += 1;
	}
	int cnt = 0;
	std::string getPath = "";
	for (unsigned long i = 0; i < _requestPath.size(); i++)
	{
		getPath += _requestPath[i];
		if (_requestPath[i] == '/')
			cnt += 1;
		if (cnt == slashCnt)
			break;
	}
	if (getPath.size() != 1 && getPath[getPath.size() - 1] == '/')
		getPath.pop_back();
	_nowLocation = _serverConfigs[k].getLocationsFind(getPath);
	_body = "";
	if (!_isFile.compare("html") || !_isFile.compare("htm") || !_isFile.compare(_nowLocation.getCgiName()))
	{
		_body = _getBody(_requestPath.substr(1), k);
		return 0;
	}
	return -1;
}

void Server::_isDirectory(int k)
{
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

// allow method 안에서 있는지 확인
void Server::_setRequestInfo(int k)
{
	_request.clear();
	_request.setRequest(_clientReq);
	if (!_request.getPath().compare("/favicon.ico"))
		_request.setRequest(_request.getMethod() + " / " + _request.getHttpVersion());
	if (!_request.getPath().compare("/redirection_from"))
		_statusCode = 302;
	_requestPath = _request.getPath();
	_requestMethod = _request.getMethod();

	//파일인 경우
	if (_fileJudge(k) == NGX_OK && _statusCode == 200)
		return;
	_isDirectory(k);
	if (_statusCode == 400 || _statusCode == 403 || _statusCode == 404 || _statusCode == 405)
	{
		_body = _setBody("400.html");
		_statusCode = 400;
	}
}

void Server::_setResponse(int k)
{
	_response.setServerName(_serverConfigs[k].getServerName());
	_response.setStatusCode(_statusCode);
	_response.setStatusMsg(_status[_statusCode]);
	if (_statusCode == 302)
		_response.setLocation("http://localhost:" + _serverConfigs[k].getServerPort() + "/redirection_to");
	_body += "\n";
	//+ content type
	_lastRespnse = _response.makeResponse(_body);
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

std::string Server::_getBody(std::string file, int k)
{
	printf("getBody file : %s", file.c_str());
	std::string body;
	std::string rootPulsFile = _nowLocation.getRoot() +  "/" + file;
	if (!_requestMethod.compare("GET"))
	{
		std::string cgiName = _nowLocation.getCgiName();
		if (!_isFile.compare("html") || !_isFile.compare("htm"))
		{
			printf("\033[31m[1]\033[37m\n");
			body = _setBody(rootPulsFile);
		}
		else if (!_isFile.compare(_nowLocation.getCgiName()))
		{
			printf("\033[31m[2]\033[37m\n");
			_cgi.execute(this->_request, _nowLocation.getCgiPath(), rootPulsFile);
			_cgi.getCgiResponseHeader();
			body = _cgi.getCgiResponseBody();
			_statusCode = _cgi.getStatusCode();
		}
		else if (_nowLocation.getAutoIndex() == 0)
		{
			printf("\033[31m[3]\033[37m\n");
			body = _response.generateAutoindexPage("./YoupiBanane" + _request.getPath());
		}
		else
			_statusCode = 403;
		printf("\033[31m[4]\033[37m\n");
		
	}
	else if (!_requestMethod.compare("POST"))
	{
		if (!_isFile.compare(_nowLocation.getCgiName()))
		{
			_cgi.execute(this->_request, _nowLocation.getCgiPath(), rootPulsFile);
			_cgi.getCgiResponseHeader();
			body = _cgi.getCgiResponseBody();
			_statusCode = _cgi.getStatusCode();
		}
		else
			_statusCode = 403;
	}
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

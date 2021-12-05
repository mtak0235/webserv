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
	_status[300] = "Multiple Choice";
	_status[302] = "Found";
	_status[400] = "Bad Request";
	_status[403] = "Forbidden";
	_status[404] = "Not Found";
	_status[405] = "Method Not Allowed";
	_status[500] = "Internal Server Error";
}

void Server::acceptNewClient(int servSock)
{
	int client_socket;
	if ((client_socket = accept(servSock, NULL, NULL)) == -1)
		_log.debugLog("accept Error");
	// std::cout << "accept new client: " << client_socket << std::endl;
	fcntl(client_socket, F_SETFL, O_NONBLOCK);

	/* add event for client socket - add read && write event */
	changeEvents(_changeList, client_socket, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	_clients[client_socket] = "";
}

int Server::recvDataFromClient(int k)
{
	_clients[_currEvent->ident] = "";
	memset(_buf, '\0', sizeof(_buf));
	while ((_readDataSize = read(_currEvent->ident, _buf, 1)) >= 0)
	{
		_buf[_readDataSize] = '\0';
		if (_buf[0] == 0)
			_clients[_currEvent->ident] += '\0';
		else
			_clients[_currEvent->ident] += _buf;
		memset(_buf, '\0', sizeof(_buf));
	}

	if (_clients[_currEvent->ident] == "")
	{
		_log.debugLog("client read error");
		disconnectClient(_currEvent->ident, _clients);
		return NGX_FAIL;
	}
	if (_responseDatatoServer(k) == NGX_FAIL)
		return NGX_FAIL;
	return NGX_OK;
}

int Server::_responseDatatoServer(int k)
{
	// _buf[_readDataSize] = '\0';
	// _clients[_currEvent->ident] += _buf;
	_clientReq = _clients[_currEvent->ident];
	std::cout << "\033[36m[RECEIVED DATA FROM " << _currEvent->ident << "]\033[37m\n" << _clientReq << std::endl;
	if (_clientReq != "")
	{
		_statusCode = 500;
		_setRequestInfo(k);
		_setResponse(k);
		std::cout << "\033[36m[RESPOND DATA" << "]\033[37m\n" << _lastRespnse << std::endl;
		if ((_n = write(_currEvent->ident, _lastRespnse.c_str(), _lastRespnse.size())) == -1)
		{
			_log.debugLog("client write error!");
			disconnectClient(_currEvent->ident, _clients);
		}
		else
			_clients[_currEvent->ident].clear();
		shutdown(_currEvent->ident, SHUT_WR);
		read(_currEvent->ident, _buf, 4000);
		close(_currEvent->ident);
		// disconnectClient(_currEvent->ident, _clients);
		return NGX_OK;
	}
	return NGX_FAIL;
}

bool Server::_isFile()
{
	_found = 0;
	_found = 	_found = _requestPath.find_last_of(".");
	if (_found == std::string::npos)
		return false;
	_fileExtension = _requestPath.substr(_found + 1);
	return true;
}

std::string Server::_getPathFromFile()
{
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
	return getPath;
}


void Server::_fileJudge(int k)
{
	std::string getPath = _getPathFromFile();
	_nowLocation = _serverConfigs[k].getLocationsFind(getPath);
	if ((int)_request.getBody().size() > _nowLocation.getCliBodySize())
	{
		_statusCode = 400;
		return;
	}
	_body = "";
	_body = _getBody(_requestPath.substr(1), k);
}

void Server::_isDirectory(int k)
{
	if (_requestPath.size() != 1 && _requestPath.back() == '/')
		_requestPath.pop_back();
	_nowLocation = _serverConfigs[k].getLocationsFind(_requestPath);
	if ((int)_request.getBody().size() > _nowLocation.getCliBodySize())
	{
		_statusCode = 400;
		return;
	}
	_allowMethods = _nowLocation.getAllowMethod();
	_isAllow = false;
	for (unsigned long i = 0; i < _allowMethods.size(); i++)
	{
		if (!_allowMethods[i].compare(_requestMethod))
			_isAllow = true;
	}

	if (!_isAllow)
	{
		_statusCode = 405;
		if (!_requestMethod.compare("GET"))
			_statusCode = 404;
	}
	else
	{
		if (_nowLocation.getAutoIndex())
		{
			std::string tmp = _response.generateAutoindexPage("./YoupiBanane" + _request.getPath());;
			if (tmp != "")
			{
				_body = tmp;
				_statusCode = 200;
			}
		}
		_indexList = _nowLocation.getIndexList();
		if (_nowLocation.getRedirectionCode() >= 300 && _nowLocation.getRedirectionCode() < 400)
			_statusCode = _nowLocation.getRedirectionCode();
		else if (_indexList.size() > 0 && !_nowLocation.getAutoIndex()) //경로인 경우
		{
			_fileExtension = "";
			_found = 0;
			_found = _indexList[0].find_last_of(".");
			_fileExtension = _indexList[0].substr(_found + 1);
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
	_requestPath = _request.getPath();
	_requestMethod = _request.getMethod();
	//path 가 파일인 경우와 경로인 경우로 나눔
	if (_isFile())
	{
		_fileJudge(k);
		return;
	}
	else
		_isDirectory(k);
	if (_statusCode == 400 || _statusCode == 403 || _statusCode == 404 || _statusCode == 405)
	{
		int tmp = _statusCode;
		_body = _setBody("400.html");
		_statusCode = tmp;
	}
}

void Server::_setResponse(int k)
{
	_response.setServerName(_serverConfigs[k].getServerName());
	_response.setStatusCode(_statusCode);
	_response.setStatusMsg(_status[_statusCode]);
	if (300 <= _statusCode && _statusCode < 400)
		_response.setLocation("http://localhost:" + _serverConfigs[k].getServerPort() + _nowLocation.getRedirectionAddress());
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
	std::string body;
	std::string root;
	if (!_nowLocation.getRoot().compare(""))
		root = "./YoupiBanane/";
	else
		root = _nowLocation.getRoot() + "/";	
	std::string rootPulsFile = root + file;
	if (!_requestMethod.compare("GET"))
	{
		if (!_fileExtension.compare(_nowLocation.getCgiName()))
		{	
			// printf("\033[31m[2]\033[37m\n");
			_cgi.execute(this->_request, _nowLocation.getCgiPath(), rootPulsFile);
			_cgi.getCgiResponseHeader();
			body = _cgi.getCgiResponseBody();
			_statusCode = _cgi.getStatusCode();
		}
		else if (_fileExtension != "")
			body = _setBody(rootPulsFile);
		else
			_statusCode = 403;
	}
	else if (!_requestMethod.compare("POST"))
	{
		if (!_fileExtension.compare(_nowLocation.getCgiName()))
		{
			std::vector<FileInfo> v = _request.getFileInfo();
			for (unsigned long i = 0; i < v.size(); i++)
			{
				std::ofstream ofs;
				std::string filePath = _nowLocation.getUploadFolder() + v[i].fileName;
				ofs.open(filePath);
				// std::cout << v[i].data.size();
				ofs.write(v[0].data.c_str(), v[i].data.size());
				ofs.close();
			}
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


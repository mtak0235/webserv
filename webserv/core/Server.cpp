#include "Server.hpp"

Server::Server() 
{
	_readDataSize = 0;
	_clientReq = "";
	_statusCode = 503;
}

Server::~Server() 
{

}

void Server::setStatus()
{
	_status[100] = "Continue";
	_status[101] = "Switching Protocols";
	_status[103] = "Early Hints";
	_status[200] = "OK";
	_status[201] = "Created";
	_status[202] = "Accepted";
	_status[203] = "Non-Authoritative Information";
	_status[204] = "No Content";
	_status[205] = "Reset Content";
	_status[206] = "Partial Content";
	_status[300] = "Multiple Choice";
	_status[301] = "Moved Permanently";
	_status[302] = "Found";
	_status[303] = "See Other";
	_status[304] = "Not Modified";
	_status[307] = "Temporary Redirect";
	_status[308] = "Permanent Redirect";
	_status[400] = "Bad Request";
	_status[401] = "Unauthorized";
	_status[402] = "Payment Required";
	_status[403] = "Forbidden";
	_status[404] = "Not Found";
	_status[405] = "Method Not Allowed";
	_status[406] = "Not Acceptable";
	_status[407] = "Proxy Authentication Required";
	_status[408] = "Request Timeout";
	_status[500] = "Internal Server Error";
	_status[501] = "Not Implemented";
	_status[502] = "Bad Gateway";
	_status[503] = "Service Unavailable";
	_status[504] = "Gateway Timeout";
	_status[505] = "HTTP Version Not Supported";

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
		_request.setRequest(_clientReq);
		_indexList = _serverConfigs[k].getLocationsFind(_request.getPath()).getIndexList();

		//정적 파일만 들어왔다고 가정
		if (_indexList.size() > 0)
		{
			std::cout << _indexList[0] << "\n";
			_ifs.open(_indexList[0]);
		}
		if (!_ifs)
		{
			_log.debugLog("file open error");
			_statusCode = 404;
			// return NGX_FAIL;
		}
		else
		{
			_body = "";
			while (_ifs.get(_c))
				_body += _c;
			_statusCode = 200;
		}
		_ifs.close();
		_response.setServerName(_serverConfigs[k].getServerName());
		_response.setStatusCode(_statusCode);
		_response.setStatusMsg(_status[_statusCode]);
		_body += "\n";
		_lastRespnse = _response.makeResponse(_body);
		std::cout <<_lastRespnse;
		int n;
		if ((n = write(_currEvent->ident, _lastRespnse.c_str(), _lastRespnse.size()) == -1))
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



#include "Server.hpp"

Server::Server()
{
  _clientReq = "";
  _body = "";
  _lastResponse = "";
  _requestMethod = "";
  _requestPath = "";
  _isFile = "";
  _c = 0;
  _isAllow = false;
  _found = 0;
  _readDataSize = 0;
  _statusCode = 500;
  _n = 0;
}

void Server::clear()
{
  _clientReq = "";
  _body = "";
  _lastResponse = "";
  _requestMethod = "";
  _requestPath = "";
  _isFile = "";
  _c = 0;
  _isAllow = false;
  _found = 0;
  _readDataSize = 0;
  _statusCode = 500;
  _n = 0;
}

Server::~Server() { }

void Server::acceptNewClient(int servSock)
{
  int client_socket;
  if ((client_socket = accept(servSock, NULL, NULL)) == -1)
    _log.debugLog("accept Error");
  fcntl(client_socket, F_SETFL, O_NONBLOCK);
  changeEvents(_changeList, client_socket, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
  _clients[client_socket] = "";
}

bool Server::_isRequestRemained(const std::string& cliReq) const {
  size_t lenReq = cliReq.length();
  if (cliReq[lenReq - 2] != '\r' && cliReq[lenReq - 1] != '\n')
    return true;
  return false;
}

int Server::recvDataFromClient(int k)
{
  memset(_buf, '\0', BUFF_SIZE);
  _readDataSize = recv(_currEvent->ident, _buf, _currEvent->data, MSG_DONTWAIT);
  if (_readDataSize == -1)
    return NGX_OK;
  else if (_readDataSize >= 0)
  {
    _clients[_currEvent->ident] += _buf;
    memset(_buf, '\0', BUFF_SIZE);
    if (_isRequestRemained(_clients[_currEvent->ident]))
      return NGX_OK;
  }
  if (_clients[_currEvent->ident] == "")
  {
    _log.debugLog("client read error");
    disconnectClient(_currEvent->ident, _clients);
  }
  if (_responseDatatoServer(k) == NGX_FAIL)
    return NGX_FAIL;
  return NGX_OK;
}

void Server::setStatus(void)
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

void Server::_setRequestInfo(int k)
{
  _request.clear();
  _request.setRequest(_clientReq);
  if (!_request.getPath().compare("/favicon.ico"))
    _request.setRequest(_request.getMethod() + " / " + _request.getHttpVersion());
  _requestPath = _request.getPath();
  _requestMethod = _request.getMethod();
  if (_fileJudge(k) == NGX_OK && _statusCode == 200)
    return;
  _isDirectory(k);
  if (_statusCode == 400 || _statusCode == 403 || _statusCode == 404 || _statusCode == 405)
  {
    int temp = _statusCode;
    _body = _setBody("400.html");
    _statusCode = temp;
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
  _lastResponse = _response.makeResponse(_body);
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
    if (!_isFile.compare(_nowLocation.getCgiName()))
    {
      _cgi.execute(this->_request, _nowLocation.getCgiPath(), rootPulsFile);
      _cgi.getCgiResponseHeader();
      body = _cgi.getCgiResponseBody();
      _statusCode = _cgi.getStatusCode();
    }
    else if (_isFile != "")
      body = _setBody(rootPulsFile);
    else
      _statusCode = 403;
  }
  else if (!_requestMethod.compare("POST"))
  {
    if (!_isFile.compare(_nowLocation.getCgiName()))
    {
      std::vector<FileInfo> v = _request.getFileInfo();
      for (unsigned long i = 0; i < v.size(); i++)
      {
        std::ofstream ofs;
        std::string filePath = _nowLocation.getUploadFolder() + v[i].fileName;
        ofs.open(filePath);
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

int Server::_responseDatatoServer(int k)
{
  _clientReq = _clients[_currEvent->ident];
	std::cout << "\033[36m[RECEIVED DATA FROM " << _currEvent->ident << "]\033[37m\n" << _clientReq << std::endl;
  if (_clientReq != "")
  {
    _statusCode = 500;
    _setRequestInfo(k);
    _setResponse(k);
		std::cout << "\033[36m[RESPOND DATA" << "]\033[37m\n" << _lastResponse << std::endl;
    if ((_n = send(_currEvent->ident, _lastResponse.c_str(), _lastResponse.size(), MSG_DONTWAIT) == -1))
    {
      std::cerr << "client write error!" << std::endl;
      disconnectClient(_currEvent->ident, _clients);
    }
    else {
      _clients[_currEvent->ident].clear();
      read(_currEvent->ident, _buf, BUFF_SIZE);
      memset(_buf, 0, sizeof(_buf));
    }
		disconnectClient(_currEvent->ident, _clients);
    return NGX_OK;
  }
  return NGX_FAIL;
}

int Server::_fileJudge(int k)
{
  _found = _requestPath.find_last_of(".");
  if (_found == std::string::npos)
    return NGX_FAIL;
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
  if ((int)_request.getBody().size() > _nowLocation.getCliBodySize())
  {
    _statusCode = 400;
    return NGX_FAIL;
  }
  _body = "";
  if (_isFile != "")
  {
    _body = _getBody(_requestPath.substr(1), k);
    return NGX_OK;
  }
  return NGX_FAIL;
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
    else if (_indexList.size() > 0 && !_nowLocation.getAutoIndex())
    {
      _isFile = "";
      _found = 0;
      _found = _indexList[0].find_last_of(".");
      _isFile = _indexList[0].substr(_found + 1);
      _body = _getBody(_indexList[0], k);
    }
  }
}

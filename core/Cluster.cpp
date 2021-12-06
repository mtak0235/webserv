#include "Cluster.hpp"

Cluster::Cluster(const std::string& configFile) {
  _makeStatusMap();
  _cntServer = 0;
  _configFile = configFile;
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

Cluster::~Cluster() { }

void Cluster::_makeStatusMap(void)
{
  _statusMap[200] = "OK";
  _statusMap[300] = "Multiple Choice";
  _statusMap[302] = "Found";
  _statusMap[400] = "Bad Request";
  _statusMap[403] = "Forbidden";
  _statusMap[404] = "Not Found";
  _statusMap[405] = "Method Not Allowed";
  _statusMap[500] = "Internal Cluster Error";
}

int Cluster::_makeServerInfo(const std::string& configFile) {
  if (_parser.parse(configFile) == FAIL) {
    Debug::log("No correct config file");
    return FAIL;
  }
  _serverInfos = _parser.getServerConfig();
  _cntServer = _serverInfos.size();
  return SUCCESS;
}

int Cluster::_makeKqueue(void)
{
  _fdEventQueue = kqueue();
  if (_fdEventQueue == -1) {
    Debug::log("kqueue() error");
    closeAllConnection();
    return FAIL;
  }
  for (size_t idxServer = 0; idxServer < _cntServer; idxServer++)
    _changeEvents(_changeList, _connector.getSockFd(idxServer), EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
  return SUCCESS;
}

void Cluster::_changeEvents(std::vector<struct kevent>& changeList,
                             uintptr_t ident, int16_t filter, uint16_t flags,
                             uint32_t fflags, intptr_t data, void *udata) {
  struct kevent tempEvent;
  EV_SET(&tempEvent, ident, filter, flags, fflags, data, udata);
  changeList.push_back(tempEvent);
}

int Cluster::_makeConnection(void) {
  for (size_t idxServer = 0; idxServer < _cntServer; idxServer++) {
    if (_connector.connect(_serverInfos[idxServer].getServerPort(), idxServer) == FAIL) {
      closeAllConnection();
      return FAIL;
    }
  }
  return SUCCESS;
}

int Cluster::init(void) {
  _makeStatusMap();
  if (_makeServerInfo(_configFile) == FAIL)
    return FAIL;
  if (_makeConnection() == FAIL)
    return FAIL;
  if (_makeKqueue() == FAIL)
    return FAIL;
  return SUCCESS;
}

void Cluster::_makeServerSocketList(void) {
  for (size_t idxServer = 0; idxServer < _cntServer; idxServer++)
    _ServerSocketList.push_back(_connector.getSockFd(idxServer));
}

int Cluster::run(void) {
  _makeServerSocketList();
  while (true) {
    if (_handleKqueueEvents(_cntServer, _ServerSocketList) == FAIL) {
      closeAllConnection();
      return FAIL;
    }
  }
  return SUCCESS;
}

int Cluster::_monitorEvents(int cntServer) {
  for (int idxServer = 0; idxServer < cntServer; idxServer++) {
    _fdOccuredEnvent[idxServer] = kevent(_fdEventQueue, &_changeList[idxServer], _changeList.size(), _eventList, 1024, NULL);
    if (_fdOccuredEnvent[idxServer] == -1) {
      Debug::log("kevent error");
      return FAIL;
    }
  }
  _changeList.clear();
  return SUCCESS;
}

int Cluster::_handleKqueueEvents(int cntServer, const std::vector<int>& serverSocketList) {
  if (_monitorEvents(cntServer) == FAIL)
    return FAIL;
  for (int idxServer = 0; idxServer < cntServer; idxServer++) {
    for (int idxEvents = 0; idxEvents < _fdOccuredEnvent[idxServer]; ++idxEvents) {
      _currEvent = &_eventList[idxEvents];
      if (_currEvent->flags & EV_ERROR) {
        if (_currEvent->ident == (uintptr_t)serverSocketList[idxServer]) {
          Debug::log("socket error");
          return FAIL;
        } else
          _disconnectClient(_currEvent->ident, _clientsMap);
      } else if (_currEvent->filter == EVFILT_READ) {
        if (_currEvent->ident == (uintptr_t)serverSocketList[idxServer])
          _acceptNewClient(serverSocketList[idxServer]);
        else if (_clientsMap.find(_currEvent->ident)!= _clientsMap.end()) {
          if (_recvDataFromClient(idxServer) == FAIL) {
            return FAIL;
          }
        }
      }
    }
  }
  return SUCCESS;
}

void Cluster::_acceptNewClient(int servSock) {
  int cliSock = 0;
  if ((cliSock = accept(servSock, NULL, NULL)) == -1)
    Debug::log("accept Error");
  fcntl(cliSock, F_SETFL, O_NONBLOCK);
  _changeEvents(_changeList, cliSock, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
  _clientsMap[cliSock] = "";
}

bool Cluster::_isRequestRemained(const std::string& cliReq) const {
  size_t lenReq = cliReq.length();
  if (cliReq[lenReq - 2] != '\r' && cliReq[lenReq - 1] != '\n')
    return true;
  return false;
}

int Cluster::_recvDataFromClient(int idxServer)
{
  memset(_buf, '\0', BUFF_SIZE);
  _readDataSize = recv(_currEvent->ident, _buf, _currEvent->data, MSG_DONTWAIT);
  if (_readDataSize == -1)
    return SUCCESS;
  else if (_readDataSize >= 0) {
    _clientsMap[_currEvent->ident] += _buf;
    memset(_buf, '\0', BUFF_SIZE);
    if (_isRequestRemained(_clientsMap[_currEvent->ident]))
      return SUCCESS;
  }
  if (_clientsMap[_currEvent->ident] == "") {
    Debug::log("client read error");
    _disconnectClient(_currEvent->ident, _clientsMap);
  }
  if (_responseDatatoServer(idxServer) == FAIL) {
    return FAIL;
  }
  return SUCCESS;
}

int Cluster::_responseDatatoServer(int idxServer) {
  _clientReq = _clientsMap[_currEvent->ident];
	std::cout << "\033[36m[RECEIVED DATA FROM " << _currEvent->ident << "]\033[37m\n" << _clientReq << std::endl;
  if (_clientReq != "") {
    _statusCode = 500;
    _makeRequestInfo(idxServer);
    _setResponse(idxServer);
		std::cout << "\033[36m[RESPOND DATA" << "]\033[37m\n" << _lastResponse << "\n";
    if ((_n = send(_currEvent->ident, _lastResponse.c_str(), _lastResponse.size(), MSG_DONTWAIT) == -1)) {
      std::cerr << "client write error!" << std::endl;
      _disconnectClient(_currEvent->ident, _clientsMap);
    } else {
      _clientsMap[_currEvent->ident].clear();
      read(_currEvent->ident, _buf, BUFF_SIZE);
      memset(_buf, 0, sizeof(_buf));
    }
		_disconnectClient(_currEvent->ident, _clientsMap);
    return SUCCESS;
  }
  return FAIL;
}











int Cluster::_fileJudge(int idxServer)
{
  _found = _requestPath.find_last_of(".");
  if (_found == std::string::npos)
    return FAIL;
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
  _nowLocation = _serverInfos[idxServer].getLocationsFind(getPath);
  if ((int)_request.getBody().size() > _nowLocation.getCliBodySize())
  {
    _statusCode = 400;
    return FAIL;
  }
  _body = "";
  if (_isFile != "")
  {
    _body = _getBody(_requestPath.substr(1), idxServer);
    return SUCCESS;
  }
  return FAIL;
}

void Cluster::_disconnectClient(int clientFd, std::map<int, std::string>& clientsMap) {
  close(clientFd);
  clientsMap.erase(clientFd);
}

int Cluster::closeAllConnection(void) {
  for (size_t idxServer = 0; idxServer < _cntServer; idxServer++)
    _connector.disconnect(idxServer);
  return SUCCESS;
}

void Cluster::_makeRequestInfo(int idxServer) {
  _request.clear();
  _request.setRequest(_clientReq);
  if (!_request.getPath().compare("/favicon.ico"))
    _request.setRequest(_request.getMethod() + " / " + _request.getHttpVersion());
  _requestPath = _request.getPath();
  _requestMethod = _request.getMethod();
  if (_fileJudge(idxServer) == SUCCESS && _statusCode == 200)
    return;
  _isDirectory(idxServer);
  if (_statusCode == 400 || _statusCode == 403 || _statusCode == 404 || _statusCode == 405)
  {
    int temp = _statusCode;
    _body = _setBody("400.html");
    _statusCode = temp;
  }
}

void Cluster::_setResponse(int idxServer)
{
  _response.setServerName(_serverInfos[idxServer].getServerName());
  _response.setStatusCode(_statusCode);
  _response.setStatusMsg(_statusMap[_statusCode]);
  if (300 <= _statusCode && _statusCode < 400)
    _response.setLocation("http://localhost:" + _serverInfos[idxServer].getServerPort() + _nowLocation.getRedirectionAddress());
  _body += "\n";
  _lastResponse = _response.makeResponse(_body);
}

std::string Cluster::_setBody(std::string file)
{
  std::ifstream ifs;
  std::string body = "";
  ifs.open(file);
  if (!ifs) {
    Debug::log("file open error");
    _statusCode = 404;
  } else {
    while (ifs.get(_c))
      body += _c;
    _statusCode = 200;
  }
  ifs.close();
  return body;
}

std::string Cluster::_getBody(std::string file, int idxServer)
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
    _serverInfos[idxServer].eraseLocation(_requestPath);
    body = "{\"success\":\"true\"}";
    _statusCode = 200;
  }
  return body;
}

void Cluster::_isDirectory(int idxServer)
{
  if (_requestPath.size() != 1 && _requestPath.back() == '/')
    _requestPath.pop_back();
  _nowLocation = _serverInfos[idxServer].getLocationsFind(_requestPath);
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
      _body = _getBody(_indexList[0], idxServer);
    }
  }
}

void Cluster::_clear()
{
  _cntServer = 0;
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

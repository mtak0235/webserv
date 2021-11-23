#include "ngxKqueue.hpp"
#include "Response.hpp"

ngxKqueue::ngxKqueue()
{

}

ngxKqueue::~ngxKqueue()
{

}

void ngxKqueue::changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
	struct kevent tempEvent;

	EV_SET(&tempEvent, ident, filter, flags, fflags, data, udata);
	changeList.push_back(tempEvent);
}

void ngxKqueue::disconnectClient(int clientFd, std::map<int, std::string>& clients)
{
    close(clientFd);
    clients.erase(clientFd);
}

int ngxKqueue::makeKqueue(std::vector<ServerConfig> &_serverConfigs)
{

	this->_serverConfigs = _serverConfigs;
	_kq = kqueue();
	if (_kq == -1)
	{
		_log.debugLog("kqueue() error");
		return NGX_FAIL;
	}
	return NGX_OK;
}

std::string openFile(std::string file_name)
{
	std::string one_line;
	std::string ret;
	std::ifstream fin(file_name);

	if (!fin.is_open())
		return NULL;
	while (std::getline(fin, one_line))
		ret = ret + one_line + "\n";
	fin.close();
	return ret;
}

void ngxKqueue::ngxKqueueInit(int servSock)
{
	changeEvents(_changeList, servSock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

int ngxKqueue::ngxKqueueProcessEvents(int x, std::vector<int> servSock)
{
	for (int i = 0; i < x; i++)
	{
		_newEvents[i] = kevent(_kq, &_changeList[i], _changeList.size(), _eventList, 1024, NULL);
		if (_newEvents[i] == -1)
		{
			_log.debugLog("kevent error");
			return NGX_FAIL;
		}
	}
	_changeList.clear();
	for (int k = 0; k < x; k++)
	{
		for (int i = 0; i < _newEvents[k]; ++i)
        {
            _currEvent = &_eventList[i];

            /* check error event return */
            if (_currEvent->flags & EV_ERROR)
            {
                if (_currEvent->ident == servSock[k])
                {
					_log.debugLog("socket error");
					return NGX_FAIL;
				}
                else
                    disconnectClient(_currEvent->ident, _clients);
            }
            else if (_currEvent->filter == EVFILT_READ)
            {
                if (_currEvent->ident == servSock[k])
                {
                    /* accept new client */
                    int client_socket;
                    if ((client_socket = accept(servSock[k], NULL, NULL)) == -1)
					{
						_log.debugLog("accept Error");
					}
                    std::cout << "accept new client: " << client_socket << std::endl;
                    fcntl(client_socket, F_SETFL, O_NONBLOCK);

                    /* add event for client socket - add read && write event */
                    changeEvents(_changeList, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    changeEvents(_changeList, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    _clients[client_socket] = "";
                }
                else if (_clients.find(_currEvent->ident)!= _clients.end())
                {
                    /* read data from client */
                    char buf[1024];
                    int n = read(_currEvent->ident, buf, sizeof(buf));

                    if (n <= 0)
                    {
                        if (n < 0)
                            std::cerr << "client read error!" << std::endl;
                        disconnectClient(_currEvent->ident, _clients);
                    }
                    else
                    {
                        buf[n] = '\0';
                        _clients[_currEvent->ident] += buf;
                        std::cout << "received data from " << _currEvent->ident << ": " << _clients[_currEvent->ident] << std::endl;
						if (_clients[_currEvent->ident] != "")
						{
							std::string test = _clients[_currEvent->ident];
							Request rq(test);
							Response rp;
							std::ifstream ifs;
							if (_serverConfigs[k].locationsFind[rq.getPath()].indexList.size() > 0)
							{
								std::cout << _serverConfigs[k].locationsFind[rq.getPath()].indexList[0] << "\n";
								ifs.open(_serverConfigs[k].locationsFind[rq.getPath()].indexList[0]);
							}
							if (!ifs)
							{
								_log.debugLog("file open error");
								return NGX_FAIL;
							}
							char c;
							std::string body = "";
							while (ifs.get(c))
								body += c;
							ifs.close();
							rp.setServerName(_serverConfigs[k].serverName);
							rp.setStatusCode(200);
							rp.setStatusMsg("OK");
							body += "\n";
							std::string res = rp.makeResponse(body);
							std::cout <<res;
							int n;
							if ((n = write(_currEvent->ident, res.c_str(), res.size()) == -1))
							{
								std::cerr << "client write error!" << std::endl;
								disconnectClient(_currEvent->ident, _clients);  
							}
							else
							{
								_clients[_currEvent->ident].clear();
								// disconnectClient(_currEvent->ident, _clients);
							}
							disconnectClient(_currEvent->ident, _clients);
						}
                    }
                }
            }
		}
	}
	return NGX_OK;
}

#include "ngx_kqueue.hpp"

ngx_kqueue::ngx_kqueue()
{

}

ngx_kqueue::~ngx_kqueue()
{

}

void ngx_kqueue::change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
	struct kevent temp_event;

	EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
	change_list.push_back(temp_event);
}

void ngx_kqueue::disconnect_client(int client_fd, std::map<int, std::string>& clients)
{
    close(client_fd);
    clients.erase(client_fd);
}

int ngx_kqueue::make_kqueue(std::vector<ServerConfig> &_serverConfigs)
{

	this->_serverConfigs = _serverConfigs;
	_kq = kqueue();
	if (_kq == -1)
	{
		_log.debug_log("kqueue() error");
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

void ngx_kqueue::ngx_kqueue_init(int servSock)
{
	change_events(_change_list, servSock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

int ngx_kqueue::ngx_kqueue_process_events(int x, int servSock)
{
	_changes = kevent(_kq, &_change_list[x], _change_list.size(), _event_list, 8, NULL);
	if (_changes == -1)
	{
		_log.debug_log("kevent error");
		return NGX_FAIL;
	}
	_change_list.clear();
	for (int i = 0; i < _changes; i++)
	{
		_curr_event = &_event_list[i];
		if (_curr_event->flags & EV_ERROR)
		{
			if (_curr_event->ident == (uintptr_t)servSock)
			{
				_log.debug_log("curr event error ");
				return NGX_FAIL;
			}
			else
			{
				_log.debug_log("client sock error");
				disconnect_client(_curr_event->ident, _clients);
			}
		}
		else if (_curr_event->filter == EVFILT_READ)
		{
			// accept
			if (_curr_event->ident == (uintptr_t)servSock)
			{
					/* accept new client */
					int client_socket;
					if ((client_socket = accept(servSock, NULL, NULL)) == -1)
					{
						_log.debug_log("accept error");
						return NGX_FAIL;
					}
					fcntl(client_socket, F_SETFL, O_NONBLOCK);
					change_events(_change_list, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
					change_events(_change_list, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
					_clients[client_socket] = "";
			}
			else if (_clients.find(_curr_event->ident)!= _clients.end())
			{
					/* read data from client */
					char buf[1024];
					int n = recv(_curr_event->ident, buf, sizeof(buf), 0);
					if (n <= 0)
					{
						if (n < 0)
						{
							_log.debug_log("client read error");
							return NGX_FAIL;
						}
						disconnect_client(_curr_event->ident, _clients);
					}
					else
					{
						Request request;
						Response response;
						std::string res;

						buf[n] = '\0';
						_clients[_curr_event->ident] += buf;
						request.parseRequest(buf);
						
						int k = _serverConfigs[x].locations.size();
						for (int i = 0; i < k; i++)
						{
							if (_serverConfigs[x].locations[i].root == request.getPath())
							{
								res = response.makeResponse(openFile(_serverConfigs[x].locations[i].indexList[0]));
								break;
							}
						}
						send(_curr_event->ident, res.c_str(), (int)strlen(res.c_str()), 0);
						close(_curr_event->ident);
					}
			}
		}
	}

	return NGX_OK;
}

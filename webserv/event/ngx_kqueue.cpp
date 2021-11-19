#include "ngx_kqueue.hpp"

ngx_kqueue::ngx_kqueue(std::string servSock)
{
	_servSock = stoi(servSock);
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

int ngx_kqueue::ngx_kqueue_init()
{
	_kq = kqueue();
	if (_kq == -1)
	{
		_log.debug_log("kqueue() error");
		return NGX_FAIL;
	}
	change_events(_change_list, _servSock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	return NGX_OK;
}

int ngx_kqueue::ngx_kqueue_process_events()
{
	_changes = kevent(_kq, &_change_list[0], _change_list.size(), _event_list, 1024, NULL);
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
			if (_curr_event->ident == (uintptr_t)_servSock)
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
			if (_curr_event->ident == (uintptr_t)_servSock)
			{
					/* accept new client */
					int client_socket;
					if ((client_socket = accept(_servSock, NULL, NULL)) == -1)
					{
						_log.debug_log("accept error");
						return NGX_FAIL;
					}
					fcntl(client_socket, F_SETFL, O_NONBLOCK);

					/* add event for client socket - add read && write event */
					change_events(_change_list, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
					change_events(_change_list, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
					_clients[client_socket] = "";
			}
			else if (_clients.find(_curr_event->ident)!= _clients.end())
			{
					/* read data from client */
					char buf[1024];
					int n = read(_curr_event->ident, buf, sizeof(buf));

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
							buf[n] = '\0';
							_clients[_curr_event->ident] += buf;
					}
			}
		}
		else if (_curr_event->filter == EVFILT_WRITE)
		{
			std::map<int, std::string>::iterator it = _clients.find(_curr_event->ident);
			if (it != _clients.end())
			{
				if (_clients[_curr_event->ident] != "")
				{
					int n;
					if ((n = write(_curr_event->ident, _clients[_curr_event->ident].c_str(),
												_clients[_curr_event->ident].size()) == -1))
					{
						_log.debug_log("write error");
						disconnect_client(_curr_event->ident, _clients);  
					}
					else
						_clients[_curr_event->ident].clear();
				}
			}
		}
	}

	return NGX_OK;
}

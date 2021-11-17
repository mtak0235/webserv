#include "Log"
#include "Server"

ConfigConfig::ServerConfig::parse()
int main(int ag, char **av)
{
	Log log;

	if (ag != 2)
	{
		log.debuglog("invalid argument counts");
		return (errorHandler());
	}
	log.debugLog("valid argument counts");

	Server server;
	if (!server.init())
		return (errorHandler);
	server.run();

	server.stop();

	isValidate()
}


#include <iostream>
#include <arpa/inet.h>
#include <unistd.h> //gethostname()
#include <netdb.h> //gethostbyname()
#include <string.h> //memcpy(), strcpy()
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <fstream>
#include <fcntl.h>
#include <map>
#include <vector>

#define PORT_NUM 8000

void error_handler(std::string msg)
{
	std::cout << msg << std::endl;
	exit(1);
}

void log_handler(std::string log)
{
	std::cout << log << std::endl;
}

int	getHostAddr()
{
	char hostName[256], hostIP[16];
	struct hostent *host;
	struct in_addr ip;

	gethostname(hostName, (int)sizeof(hostName));
	host = gethostbyname(hostName);
	for (int i = 0; host->h_addr_list[i]; i++)
		memcpy(&ip, host->h_addr_list[i], 4);;
	strcpy(hostName, inet_ntoa(ip));
	std::cout << "build server : http://" << hostName << ":" << PORT_NUM << std::endl;
	return 0;
}

void getRequestPath(std::string &path, char *request)
{
	if (!strncmp(request, "GET ", 4))
		path = strtok(request + 4, " ");
	else if (!strncmp(request, "POST ", 5))
		path = strtok(request + 5, " ");
	else if (!strncmp(request, "DELETE ", 6))
		path = strtok(request + 6, " ");
	log_handler("request path : " + path);
}

std::string openFile(std::string file_name)
{
	std::string one_line;
	std::string ret;
	std::ifstream fin(file_name);

	if (!fin.is_open())
	{
		std::cout << "failed to generate " << file_name << "on return" << std::endl;
		return NULL;
	}
	while (std::getline(fin, one_line))
		ret = ret + one_line + "\n";
	fin.close();
	return ret;
}

std::string getResponseHeader()
{
	return openFile("header");
}

std::string getResponseBody(std::string path)
{
	std::string ret;

	if (!strcmp(path.c_str(), "/hello"))
		ret = openFile("hello.html");
	else if(!strcmp(path.c_str(), "/world"))
		ret = openFile("world.html");
	return ret;
}

void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent temp_event;
	//kevent 초기화
    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(temp_event);
}

void disconnect_client(int client_fd, std::map<int, std::string>& clients)
{
    std::cout << "client disconnected: " << client_fd << std::endl;
    close(client_fd);
    clients.erase(client_fd);
}

int main()
{
	int server_sock;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	char received_msg[1024];


	if (getHostAddr())
	{
		printf("Failed occupying IP address");
		return -1;
	}	
	//서버 소켓 만듦.
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == -1)
		error_handler("socket error");
	
	//서버 소켓 설정
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT_NUM);
	//서버 소켓 바인딩
	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		error_handler("bind error");
	log_handler("binding successful");
	//요청 대기 시작
	if (listen(server_sock, 5) == -1)
		error_handler("listen error");
	log_handler("listening successful");
	//굳이 이게 필요할까?accept에서 대기하지 말라는 의도인 것 같은데 
	fcntl(server_sock, F_SETFL, O_NONBLOCK);
	//kernel님 이벤트 저장할 큐 만들어주세요
	int kq;
    if ((kq = kqueue()) == -1)
		error_handler("kqueue err");
	
	//서버 소켓에 대한 이벤트 추가
	std::vector<struct kevent> change_list;
    change_events(change_list, server_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

	int new_events;
	struct kevent* curr_event;
	struct kevent event_list[8]; 
	std::map<int, std::string> clients;
	//요청 처리
	while (true)
	{
		//새로 모니터링할 이벤트 등록하고, 처리아직 안된 이벤트 갯수 리턴
		new_events = kevent(kq, &change_list[0], change_list.size(), event_list, 8, NULL);
		if (new_events == -1)
			error_handler("kevent err");
		change_list.clear();
		for (int i = 0; i < new_events; ++i)
		{
			curr_event = &event_list[i];
			if (curr_event->flags & EV_ERROR)
			{
				if (curr_event->ident == server_sock)
                    error_handler("server socket error");
                else
                {
                    std::cerr << "client socket error" << std::endl;
                    disconnect_client(curr_event->ident, clients);
                }
			}
			else if (curr_event->filter == EVFILT_READ)
			{
				if (curr_event->ident == server_sock)
				{
					int client_sock;
					socklen_t client_addr_size = sizeof(client_addr);
					client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
					if (client_sock == -1)
						error_handler("accept error");
					log_handler("Accepting successful");
					fcntl(client_sock, F_SETFL, O_NONBLOCK);

					change_events(change_list, client_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    change_events(change_list, client_sock, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    clients[client_sock] = "";
				}
				else if (clients.find(curr_event->ident)!= clients.end())
				{
					int received_msg_len = recv(curr_event->ident, received_msg, sizeof(received_msg) - 1, 0);
					if (received_msg_len <= 0)
                    {
                        if (received_msg_len < 0)
                            std::cerr << "client read error!" << std::endl;
                        disconnect_client(curr_event->ident, clients);
                    }
                    else
                    {
                        received_msg[received_msg_len] = '\0';
						// log_handler(received_msg);
						std::string path;
						getRequestPath(path, received_msg);
						std::string response_header = getResponseHeader();
						// send(client_sock, &response_header, (int)strlen(response_header.c_str()), 0);
						std::string response_body = getResponseBody(path);
						std::string response = response_header + "\n" + response_body;
						std::cout << "\033[32m";
						log_handler(response);
						std::cout << "\033[37m";
                        clients[curr_event->ident] += response;
                        std::cout << "received data from " << curr_event->ident << ": " << clients[curr_event->ident] << std::endl;
                    }
				}
			}
			else if (curr_event->filter == EVFILT_WRITE)
			{
				std::map<int, std::string>::iterator it = clients.find(curr_event->ident);
				if (it != clients.end())
				{
					if (clients[curr_event->ident] != "")
					{
						if (send(curr_event->ident, clients[curr_event->ident].c_str(), clients[curr_event->ident].size(), 0) == -1)
						{
							std::cerr << "failed to send" << std::endl;
							disconnect_client(curr_event->ident, clients);
						}
						else
							clients[curr_event->ident].clear();
					}
				}
			}
		}
	}
	return 0;
}

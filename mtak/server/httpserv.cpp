#include <iostream>
#include <arpa/inet.h>
#include <unistd.h> //gethostname()
#include <netdb.h> //gethostbyname()
#include <string.h> //memcpy(), strcpy()
#include <sys/socket.h>
#include <fstream>

struct sockaddr_in server_addr;
struct sockaddr_in client_addr;
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

	// if (WSAStartup(MAKEWORD(1, 1), &wsaData) != NO_ERROR)
	// {
	// 	printf("wsaData: WSAStartup() failed");
	// 	return -1;
	// }
	gethostname(hostName, (int)sizeof(hostName));
	host = gethostbyname(hostName);
	for (int i = 0; host->h_addr_list[i]; i++)
		memcpy(&ip, host->h_addr_list[i], 4);;
	strcpy(hostName, inet_ntoa(ip));
	std::cout << "build server : http://" << hostName << ":" << PORT_NUM << std::endl;
	// WSACleanup();
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
int main()
{
	int server_sock;
	int client_sock;
	struct sockaddr_in server_addr;
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
	//요청 처리
	while (true)
	{
		socklen_t client_addr_size = sizeof(client_addr);
		client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
		if (client_sock == -1)
			error_handler("accept error");
		log_handler("Accepting successful");
		int received_msg_len = recv(client_sock, received_msg, sizeof(received_msg) - 1, 0);
		received_msg[received_msg_len] = 0;
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
		// send(client_sock, &response_body, (int)strlen(response_body.c_str()), 0);
		if (send(client_sock, response.c_str(), (int)strlen(response.c_str()), 0) < 1)
			error_handler("failed to send");
		close(client_sock);
	}
	close(server_sock);
	return 0;
}

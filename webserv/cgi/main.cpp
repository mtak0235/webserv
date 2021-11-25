#include "Cgi.hpp"
#include "../http/Request.hpp"

#define PORT_NUM 8001

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

std::string getResponseHeader()
{
	return openFile("header");
}

void getRequestPath(std::string &path, char *request)
{
	if (!strncmp(request, "GET ", 4))
		path = strtok(request + 4, " ");
	else if (!strncmp(request, "POST ", 5))
		path = strtok(request + 5, " ");
	else if (!strncmp(request, "DELETE ", 6))
		path = strtok(request + 6, " ");
	std::cout << "request path : " + path << std::endl;
}

std::string getResponseBody(Request req)
{
	std::string ret;

	if (!strcmp(req.getPath().c_str(), "/cgi-tester"))
    {
        Cgi tester(&req);
        tester.execute(tester.getEnv(),"hELLOwORLD", "./cgi_tester");
        ret = tester.getContent();
    }
	else if(!strcmp(req.getPath().c_str(), "/php-tester"))
    {
        Cgi php(&req);
        
    }
	return ret;
}

int main()
{
    int serv_sock;
   	struct sockaddr_in serv_addr; 
    fd_set reads;
    int fd_max;
    fd_set temps;
    struct timeval timeout;
    int result;

    getHostAddr();
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serv_addr.sin_port = htons(PORT_NUM);
    bind(serv_sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 5);
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while (true)
    {
        int clnt_sock;

        temps=reads;
        timeout.tv_sec = 5;
		timeout.tv_usec = 0;
        result = select(fd_max+1, &temps, 0,0,&timeout);
        if(result == -1) break; 
        if(result == 0) continue;
        for(int fd = 0 ; fd < fd_max+1 ; fd++)
        {
            if(FD_ISSET(fd, &temps) == 0) continue;
            if(fd != serv_sock)//client 소켓이면
            {
                char msg[1024];
                int read_len = recv(fd,msg,sizeof(msg) - 1, 0); 
                msg[read_len] = 0;
                Request req(msg);
                std::string path;

                getRequestPath(path, msg);
                std::string response = getResponseHeader() + "\n" + getResponseBody(req);
                std::cout << "\033[34m" <<  response << "\033[37m" << std::endl;
                send(fd, response.c_str(), (int)strlen(response.c_str()), 0);
                printf("Disconnect client : %d \n", fd);
                FD_CLR(fd, &reads);
                close(fd);
                if(fd_max == fd) fd_max--;
            }
            else//server socket이면
            {
                struct sockaddr_in clnt_addr; 
                socklen_t clnt_addr_size = sizeof(clnt_addr);
				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); 
				if(clnt_sock == -1)
					continue;
				FD_SET(clnt_sock, &reads);
				if(fd_max<clnt_sock)
					fd_max = clnt_sock; 
				printf("Connect New Client : %d \n",clnt_sock); 
            }
        }
    }
    close(serv_sock);
    return 0;
}
#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Cgi.hpp"
#include "Config.hpp"
#include "Connector.hpp"
#include "Debug.hpp"
#include "Parser.hpp"
#include "Response.hpp"
#include "Request.hpp"

#define BUFF_SIZE 2048
#define SUCCESS 0
#define FAIL -1

class Cluster
{
  public:
    /* constructor & destructor */
    Cluster(const std::string& configFile);
    ~Cluster();

    /* public function */
    int init(void);
    int run(void);
    int closeAllConnection(void);


  private:
    int _makeConnection(void);
    int _makeServerInfo(const std::string& configFIle);
    int _makeKqueue(void);
    int _recvDataFromClient(int idxServer);
    int _handleKqueueEvents(int cntServer, const std::vector<int>& servSock);
    int _responseDatatoServer(int idxServer, char* buff);
    int _fileJudge(int idxServer);
    int _monitorEvents(int cntServer);
    void _makeStatusMap(void);
    void _changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident,
                      int16_t filter, uint16_t flags, uint32_t fflags,
                      intptr_t data, void *udata);
    void _makeServerSocketList(void);
    void _disconnectClient(int clientFd, std::map<int, std::string>& clientsMap);
    void _acceptNewClient(int servSock);
    void  _makeRequestInfo(int idxServer, const std::string& cliReq);
    void _setResponse(int idxServer);
    std::string _setBody(std::string file);
    std::string _getBody(std::string file, int idxServer);
    void _isDirectory(int idxServer);
    bool _isRequestRemained(const std::string& cliReq) const;
		void _clear();


  private:
    /* private variable */
    Parser _parser;
    Connector _connector;
    Request _request;
    Response _response;
    Cgi _cgi;
    LocationConfig _nowLocation;

    std::map<int, std::string> _clientsReqMap;
    std::map<int, std::string> _statusMap;
    std::vector<ServerConfig> _serverInfos;
    std::vector<int> _ServerSocketList;

    struct kevent* _currEvent;
    int _fdEventQueue;
    int _fdOccuredEnvent[6];
    struct kevent _eventList[1024];
    std::vector<struct kevent> _changeList;

    std::string _configFile;
    std::string _body;
    std::string _lastResponse;
    std::string _isFile;
};

#endif

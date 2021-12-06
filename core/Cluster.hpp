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
    void _makeStatusMap(void);
    int _makeServerInfo(const std::string& configFIle);
    int _makeKqueue(void);
    void _changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident,
                      int16_t filter, uint16_t flags, uint32_t fflags,
                      intptr_t data, void *udata);


    int ngxKqueueProcessEvents(int x, std::vector<int> servSock);
    void disconnectClient(int clientFd, std::map<int, std::string>& clients);
    void acceptNewClient(int servSock);
    int recvDataFromClient(int k);
    void _setRequestInfo(int k);
    void _setResponse(int k);
    std::string _setBody(std::string file);
    std::string _getBody(std::string file, int k);
    int _responseDatatoServer(int k);
    int _fileJudge(int k);
    void _isDirectory(int k);
    bool _isRequestRemained(const std::string& cliReq) const;
		void clear();


  private:
    /* private variable */
    std::string _configFile;
    std::ifstream _ifs;

    Parser _parser;
    std::vector<ServerConfig> _serverInfos;
    std::map<int, std::string> _statusMap;
    Connector _connector;
    Cgi _cgi;
    Request _request;
    Response _response;
    LocationConfig _nowLocation;

    struct kevent* _currEvent;
    int _FdEventQueue;
    int _newEvents[6];
    struct kevent _eventList[1024];
    std::vector<struct kevent> _changeList;

    std::map<int, std::string> _clients;
    std::string _clientReq;
    std::string _body;
    std::string _lastResponse;
    std::string _requestMethod;
    std::string _requestPath;
    std::string _isFile;
    std::vector<std::string> _allowMethods;
    std::vector<std::string> _indexList;

    char _buf[BUFF_SIZE];
    char _c;
    bool _isAllow;
    size_t _found;
    int _readDataSize;
    int _statusCode;
    int _n;
};

#endif

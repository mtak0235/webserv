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
#include "Connection.hpp"
#include "Log.hpp"
#include "Parser.hpp"
#include "Response.hpp"
#include "Request.hpp"

#define NGX_OK 0
#define NGX_FAIL -1

class Cluster
{
  public:
    /* constructor & destructor */
    Cluster();
    virtual ~Cluster();

    /* public function */
    int init(const std::string& confFile);
    int run(void);
    int stop(void);

    /* pure virtual function */
    virtual int makeKqueue() = 0;
    virtual void ngxKqueueInit(int servSock) = 0;
    virtual int ngxKqueueProcessEvents(int x, std::vector<int> servSock) = 0;
    virtual void setStatus() = 0;

  protected:
    /* protected variable */
    std::vector<ServerConfig> _serverConfigs;
    std::map<int, std::string> _status;
    Log _log;

  private:
    /* private variable */
    Parser _parser;
    Connection _connection;
};

#endif

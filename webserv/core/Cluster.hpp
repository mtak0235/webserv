#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "core.hpp"
#include "Connection.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "Log.hpp"
#include "ngxKqueue.hpp"

#include <iostream>
#include <vector>

class Cluster
{
private:
    Parser _parser;
    Connection _connection;
    ngxKqueue _ngxKqueue;
protected:
    std::vector<ServerConfig> _serverConfigs;
	Log _log;
public:
    Cluster(void);
    ~Cluster();
    int init(const std::string& confFile);
    int run(void);
    int stop(void);
};

#endif
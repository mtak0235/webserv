#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "core.hpp"
#include "Connection.hpp"
#include "ngx_kqueue.hpp"
#include "Server.hpp"
#include "Parser.hpp"

#include <iostream>
#include <vector>

class Cluster
{
private:
    Parser _parser;
    Connection _connection;
    std::vector<ServerConfig> _serverConfigs;
    ngx_kqueue _ngx_kqueue;
public:
    Cluster(void);
    ~Cluster();
    int init(const std::string& confFile);
    int run(void);
    int stop(void);
};

#endif
#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "core.hpp"
#include "Connection.hpp"
#include "Parser.hpp"
#include "Log.hpp"

#include <iostream>
#include <vector>

class Cluster
{
private:
    Parser _parser;
    Connection _connection;
protected:
    std::vector<ServerConfig> _serverConfigs;
		std::map<int, std::string> _status; 
		Log _log;
public:
	Cluster(void);
	virtual ~Cluster();
	int init(const std::string& confFile);
	int run(void);
	int stop(void);

	virtual int makeKqueue() = 0;
	virtual void ngxKqueueInit(int servSock) = 0;
	virtual int ngxKqueueProcessEvents(int x, std::vector<int> servSock) = 0;
	virtual void setStatus() = 0;
  	// virtual void ngxKqueueStop() = 0;
};

#endif
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <string>

struct LocationConfig
{
	std::string locationName;
	std::string root;
	std::vector<std::string> indexList;
	std::vector<std::string> allowMethods;
	std::string cgiName;
	std::string cgiPath;
	int cliBodySize;
	std::string uploadFolder;
};

struct ServerConfig
{
	std::string serverName;
	int serverPort;
	std::vector<LocationConfig> locations;
};

#endif

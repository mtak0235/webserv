#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

class LocationConfig
{
private:
	std::string _locationName;
	std::string _root;
	std::vector<std::string> _indexList;
	std::vector<std::string> _allowMethods;
	std::string _cgiName;
	std::string _cgiPath;
	int _cliBodySize;
	std::string _uploadFolder;
public:
	LocationConfig();
	// LocationConfig(LocationConfig &x);
	~LocationConfig();
	LocationConfig operator=(LocationConfig &x);
	void setLocationName(const std::string locationName);
	void setRoot(const std::string root);
	void popIndexList();
	void setIndexList(std::string index);
	void popAllowMethod();
	void setAllowMethod(std::string allowMethod);
	void setCgiName(std::string cgiName);
	void setCgiPath(std::string cgiPath);
	void setCliBodySize(int cliBodySize);
	void setUploadFolder(std::string uploadFolder);

	std::string getLocationName();
	std::string getRoot();
	std::vector<std::string> getIndexList();
	std::vector<std::string> getAllowMethod();
	std::string getCgiName();
	std::string getCgiPath();
	int getCliBodySize();
	std::string getUploadFolder();

	bool empty();
};

class ServerConfig
{
private:
	std::string _serverName;
	std::string _serverPort;
	std::vector<LocationConfig> _locations;
	std::map<std::string, LocationConfig> _locationsFind;
public:
	ServerConfig();
	~ServerConfig();
	ServerConfig operator=(ServerConfig &x);
	void setServerName(const std::string serverName);
	void setServerPort(const std::string serverPort);
	void setLocations(LocationConfig location);
	void setLocationsFind(std::string locationName, LocationConfig config);

	std::string getServerName();
	std::string getServerPort();
	std::vector<LocationConfig> getLocations();
	LocationConfig &getLocationsFind(std::string locationName);
};

#endif

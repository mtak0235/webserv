#include "Config.hpp"

LocationConfig::LocationConfig()
{
	_locationName = "";
	_root = "";
	_cgiName = "";
	_cgiPath = "";
	_cliBodySize = 0;
	_uploadFolder = "";
}

LocationConfig::~LocationConfig()
{

}

LocationConfig LocationConfig::operator=(LocationConfig &x)
{
	_locationName = x._locationName;
	_root = x._root;
	_cgiName = x._cgiName;
	_cgiPath = x._cgiPath;
	_cliBodySize = x._cliBodySize;
	_uploadFolder = x._uploadFolder;
	_indexList = x._indexList;
	_allowMethods = x._allowMethods;
	return *this;
}

void LocationConfig::setLocationName(const std::string locationName)
{
	_locationName = locationName;
}

void LocationConfig::setRoot(const std::string root)
{
	_root = root;
}

void LocationConfig::setIndexList(std::string index)
{
	_indexList.push_back(index);
}

void LocationConfig::popIndexList()
{
	_indexList.pop_back();
}

void LocationConfig::popAllowMethod()
{
	_allowMethods.pop_back();
}

void LocationConfig::setAllowMethod(std::string allowMethod)
{
	_allowMethods.push_back(allowMethod);
}

void LocationConfig::setCgiName(std::string cgiName)
{
	_cgiName = cgiName;
}

void LocationConfig::setCgiPath(std::string cgiPath)
{
	_cgiPath = cgiPath;
}

void LocationConfig::setCliBodySize(int cliBodySize)
{
	_cliBodySize = cliBodySize;
}

void LocationConfig::setUploadFolder(std::string uploadFolder)
{
	_uploadFolder = uploadFolder;
}

void LocationConfig::setAutoIndex(std::string autoindex)
{
	if (!autoindex.compare("on"))
		_autoindex = ON;
	else
		_autoindex = OFF;
}
std::string LocationConfig::getLocationName()
{
	return _locationName;
}

std::string LocationConfig::getRoot()
{
	return _root;
}

std::vector<std::string> LocationConfig::getIndexList()
{
	return _indexList;
}

std::vector<std::string> LocationConfig::getAllowMethod()
{
	return _allowMethods;
}

std::string LocationConfig::getCgiName()
{
	return _cgiName;
}

std::string LocationConfig::getCgiPath()
{
	return _cgiPath;
}

int LocationConfig::getCliBodySize()
{
	return _cliBodySize;
}

std::string LocationConfig::getUploadFolder()
{
	return _uploadFolder;
}

bool LocationConfig::empty()
{
	if (_locationName == "")
		return true;
	return false;
}

ServerConfig::ServerConfig()
{
	_serverName = "";
	_serverPort = "";
}

ServerConfig::~ServerConfig()
{

}

ServerConfig ServerConfig::operator=(ServerConfig &x)
{
	_serverName = x._serverName;
	_serverPort = x._serverPort;
	_locations = x._locations;
	_locationsFind = x._locationsFind;
	return *this;
}

void ServerConfig::setServerName(const std::string serverName)
{
	_serverName = serverName;
}

void ServerConfig::setServerPort(const std::string serverPort)
{
	_serverPort = serverPort;
}

void ServerConfig::setLocations(LocationConfig location)
{
	_locations.push_back(location);
}

void ServerConfig::setLocationsFind(std::string locationName, LocationConfig config)
{
	_locationsFind[locationName] = config;
}

std::string ServerConfig::getServerName()
{
	return _serverName;
}

std::string ServerConfig::getServerPort()
{
	return _serverPort;
}

std::vector<LocationConfig> ServerConfig::getLocations()
{
	return _locations;
}

LocationConfig &ServerConfig::getLocationsFind(std::string locationName)
{
	return _locationsFind[locationName];
}

void ServerConfig::eraseLocation(std::string locationName)
{
	_locationsFind.erase(locationName);
}


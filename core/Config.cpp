#include "Config.hpp"

/************************************************************/
/*                   LocationConfig Class                   */
/************************************************************/

LocationConfig::LocationConfig()
{
	_locationName = "";
	// _allowMethods 초기화 빠짐
	_root = "";
	// _indexList 초기화 빠짐
	_cgiName = "";
	_cgiPath = "";
	_uploadFolder = "";
	_autoindex = false;
	_redirectCode = 0;
	_redirectAddress = "";
	_cliBodySize = 10000;
}

LocationConfig::~LocationConfig() { }


void LocationConfig::clear(void)
{
	_locationName = "";
	_allowMethods.clear();
	_root = "";
	_indexList.clear();
	_cgiName = "";
	_cgiPath = "";
	_uploadFolder = "";
	_autoindex = false;
	_redirectCode = 0;
	_redirectAddress = "";
	_cliBodySize = 10000;
}

bool LocationConfig::empty(void)
{
	if (_locationName == "")
		return true;
	return false;
}

void LocationConfig::popIndexList(void)
{
	_indexList.pop_back();
}

void LocationConfig::popAllowMethod(void)
{
	_allowMethods.pop_back();
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

void LocationConfig::setAllowMethod(std::vector<std::string> methods)
{
	_allowMethods = methods;
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
		_autoindex = true;
	else
		_autoindex = false;
}

void LocationConfig::setRedirectionCode(int redirectionCode)
{
	_redirectCode = redirectionCode;
}

void LocationConfig::setRedirectionAddress(std::string redirectionAddress)
{
	_redirectAddress = redirectionAddress;
}

std::string LocationConfig::getLocationName(void)
{
	return _locationName;
}

std::string LocationConfig::getRoot(void)
{
	return _root;
}

std::vector<std::string> LocationConfig::getIndexList(void)
{
	return _indexList;
}

std::vector<std::string> LocationConfig::getAllowMethod(void)
{
	return _allowMethods;
}

std::string LocationConfig::getCgiName(void)
{
	return _cgiName;
}

std::string LocationConfig::getCgiPath(void)
{
	return _cgiPath;
}

int LocationConfig::getCliBodySize(void)
{
	return _cliBodySize;
}

std::string LocationConfig::getUploadFolder(void)
{
	return _uploadFolder;
}

int LocationConfig::getAutoIndex(void)
{
	return _autoindex;
}


int LocationConfig::getRedirectionCode(void)
{
	return _redirectCode;
}

std::string LocationConfig::getRedirectionAddress(void)
{
	return _redirectAddress;
}



/************************************************************/
/*                    ServerConfig Class                    */
/************************************************************/

ServerConfig::ServerConfig()
{
	_serverName = "webserv";
	_serverPort = "";
	// _locations 초기화 빠짐
	// _locationsFind 초기화 빠짐
	// _errorPage 초기화 빠짐
}

ServerConfig::~ServerConfig() { }

void ServerConfig::clear(void)
{
	_serverName = "";
	_serverPort = "";
	_locations.clear();
	_locationsFind.clear();
	// _errorPage 클리어 빠짐
}

void ServerConfig::eraseLocation(std::string locationName)
{
	_locationsFind.erase(locationName);
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

void ServerConfig::setErrorPage(std::string errorPage)
{
	_errorPage = errorPage;
}

std::string ServerConfig::getServerName(void)
{
	return _serverName;
}

std::string ServerConfig::getServerPort(void)
{
	return _serverPort;
}

std::vector<LocationConfig> ServerConfig::getLocations(void)
{
	return _locations;
}

LocationConfig &ServerConfig::getLocationsFind(std::string locationName)
{
	return _locationsFind[locationName];
}



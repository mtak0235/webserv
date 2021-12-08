#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

class LocationConfig
{
  public:
    /* constructor & destructor */
    LocationConfig();
    ~LocationConfig();


    /* public functions */
    void clear(void);
    bool empty(void);
    void popIndexList(void);
    void popAllowMethod(void);

    /* getter & setter */
    void setLocationName(const std::string locationName);
    void setRoot(const std::string root);
    void setIndexList(std::string index);
    void setAllowMethod(std::vector<std::string> methods);
    void setCgiName(std::string cgiName);
    void setCgiPath(std::string cgiPath);
    void setCliBodySize(int cliBodySize);
    void setUploadFolder(std::string uploadFolder);
    void setAutoIndex(std::string autoindex);
    void setRedirectionCode(int redirectionCode);
    void setRedirectionAddress(std::string redirectionAddress);
    std::string getLocationName(void);
    std::string getRoot(void);
    std::vector<std::string> getIndexList(void);
    std::vector<std::string> getAllowMethod(void);
    std::string getCgiName(void);
    std::string getCgiPath(void);
    int getCliBodySize(void);
    std::string getUploadFolder(void);
    int getAutoIndex(void);
    int getRedirectionCode(void);
    std::string getRedirectionAddress(void);

  private:
    /* private variable */
    std::string _locationName;
    std::vector<std::string> _allowMethods;
    std::string _root;
    std::vector<std::string> _indexList;
    std::string _cgiName;
    std::string _cgiPath;
    std::string _uploadFolder;
    bool _autoindex;
    int _redirectCode;
    std::string _redirectAddress;
    int _cliBodySize;
};

class ServerConfig
{
  public:
    /* constructor & destructor */
    ServerConfig();
    ~ServerConfig();


    /* public functions */
    void clear(void);
    void eraseLocation(std::string locationName);

    /* getter & setter */
    void setServerName(const std::string serverName);
    void setServerPort(const std::string serverPort);
    void setLocations(LocationConfig location);
    void setLocationsFind(std::string locationName, LocationConfig config);
    void setErrorPage(std::string errorPage);
    std::string getServerName(void);
    std::string getServerPort(void);
    std::vector<LocationConfig> getLocations(void);
    LocationConfig& getLocationsFind(std::string locationName);

  private:
    /* private variable */
    std::string _serverName;
    std::string _serverPort;
    std::vector<LocationConfig> _locations;
    std::map<std::string, LocationConfig> _locationsFind;
    std::string _errorPage;
};

#endif

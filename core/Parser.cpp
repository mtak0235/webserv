#include "Parser.hpp"

const std::string Parser::_keyInit = "server";
const std::string Parser::_keyServer[3] = {"listen", "server_name", "location"};
const std::string Parser::_keyLocation[9] = {"autoindex","client_max_body_size", "method", "index",
                                         "root", "cgi_extension", "cgi_path", "upload_folder", "return"};

Parser::Parser(void) { }

Parser::~Parser() { }

const std::string Parser::getConfigFile(int argc, char* argv[])
{
	std::string configFile;
	if (argc == 1)
	  configFile = "./basic.conf";
	else if (argc == 2)
		configFile = argv[1];
	return configFile;
}

int Parser::parse(const std::string& confFile)
{
	size_t found = confFile.find_last_of(".");
	if (found == std::string::npos)
	{
		_log.debugLog("not conf file");
		return PARSE_FAIL;
	}
	std::string tmp = confFile.substr(found + 1);
	if (tmp.compare("conf"))
	{
		_log.debugLog("not conf file");
		return PARSE_FAIL;
	}
	_ifs.open(confFile);
	std::vector<std::string> file;
	while (!_ifs.eof())
	{
		std::string line;
		getline(_ifs, line);
		file.push_back(line);
	}
	_ifs.close();
	int serverBlock = 0;
	int locationBlock = 0;
	ServerConfig sc;
	LocationConfig lc;
	for (size_t i = 0; i < file.size(); i++)
	{
		std::stringstream ss(file[i]);
		_info = "";
		ss >> _info;
		if (_info == "server")
		{
			ss >> _info;
			if (_info != "{")
				return PARSE_FAIL;
			serverBlock = 1;
			continue;
		}
		else if (serverBlock == 1 && _info == "listen")
		{
			ss >> _info;
			if (_info.back() != ';')
			{
				_log.debugLog("listen semicolon error");
				return PARSE_FAIL;
			}
			_info.pop_back();
			for (size_t k = 0; k < _info.size(); k++)
			{
				if (!isnumber(_info[k]))
				{
					_log.debugLog("port num is not number");
					return PARSE_FAIL;
				}
			}
			sc.setServerPort(_info);
		}
		else if (serverBlock == 1 && _info == "server_name")
		{
			ss >> _info;
			if (_info.back() != ';')
			{
				_log.debugLog("server name semi error");
				return PARSE_FAIL;
			}
			_info.pop_back();
			sc.setServerName(_info);
		}
		else if (serverBlock == 1 && _info == "error_page")
		{
			ss >> _info;
			if (_info.back() != ';')
			{
				_log.debugLog("server name semi error");
				return PARSE_FAIL;
			}
			_info.pop_back();
			sc.setErrorPage(_info);
		}
		else if (serverBlock == 1 && _info == "location")
		{
			std::string bracket;
			ss >> _info >> bracket;
			if (bracket != "{")
			{
				_log.debugLog("location start bracket error");
				return PARSE_FAIL;
			}
			locationBlock = 1;
			lc.setLocationName(_info);
		}
		else if (serverBlock == 1 && locationBlock == 1)
		{
			if (_info == "root")
			{
				if (_getLocationInfo(ss, "root parsing error") == PARSE_FAIL)
					return PARSE_FAIL;
				lc.setRoot(_info);
			}
			else if (_info == "index")
			{
				if (_getLocationInfo(ss, "index parsing error") == PARSE_FAIL)
					return PARSE_FAIL;
				lc.setIndexList(_info);
			}
			else if (_info == "method")
			{
				std::string method;
				std::vector<std::string> tmp;
				while (ss >> method)
					tmp.push_back(method);
				if (tmp[tmp.size() - 1].back() != ';')
				{
					_log.debugLog("method semi error");
					return PARSE_FAIL;
				}
				tmp[tmp.size() - 1].pop_back();
				for (size_t k = 0; k < tmp.size(); k++)
				{
					if (tmp[k] != "GET" && tmp[k] != "POST" && tmp[k] != "DELETE")
					{
						_log.debugLog("not correct method");
						return PARSE_FAIL;
					}
				}
				lc.setAllowMethod(tmp);
			}
			else if (_info == "cgi_path")
			{
				if (_getLocationInfo(ss, "cgi path parsing error") == PARSE_FAIL)
					return PARSE_FAIL;
				lc.setCgiPath(_info);
			}
			else if (_info == "cgi_extension")
			{
				if (_getLocationInfo(ss, "cgi extension parsing error") == PARSE_FAIL)
					return PARSE_FAIL;
				lc.setCgiName(_info);
			}
			else if (_info == "client_max_body_size")
			{
				if (_getLocationInfo(ss, "cgi extension parsing error") == PARSE_FAIL)
					return PARSE_FAIL;
				for (size_t k = 0; k < _info.size(); k++)
				{
					if (!isnumber(_info[k]))
					{
						_log.debugLog("client body size is not number");
						return PARSE_FAIL;
					}
				}
				lc.setCliBodySize(stoi(_info));
			}
			else if (_info == "upload_folder")
			{
				if (_getLocationInfo(ss, "cgi extension parsing error") == PARSE_FAIL)
					return PARSE_FAIL;
				lc.setUploadFolder(_info);
			}
			else if (_info == "autoindex")
			{
				if (_getLocationInfo(ss, "cgi extension parsing error") == PARSE_FAIL)
					return PARSE_FAIL;
				lc.setAutoIndex(_info);
			}
			else if (_info == "return")
			{
				int code;
				std::string address;
				ss >> code >> address;
				if (code < 300 || code >= 400)
				{
					_log.debugLog("return parsing error: not correct status code ");
					return PARSE_FAIL;
				}
				if (address.back() != ';')
				{
					_log.debugLog("return semi error");
					return PARSE_FAIL;
				}
				address.pop_back();
				lc.setRedirectionCode(code);
				lc.setRedirectionAddress(address);
			}
			else if (_info == "}")
			{
				locationBlock = 0;
				sc.setLocations(lc);
				sc.setLocationsFind(lc.getLocationName(), lc);
				lc.clear();
			}
			else
			{
				_log.debugLog("Not location block keyword");
				return PARSE_FAIL;
			}
		}
		else if (serverBlock == 1 && locationBlock == 0 && _info == "}")
		{
			serverBlock = 0;
			_serverConfigs.push_back(sc);
			sc.clear();
		}
		else if (_info == "")
			continue;
		else
		{
			_log.debugLog("No keyword");
			return PARSE_FAIL;
		}
	}
	if (serverBlock != 0 || locationBlock != 0)
		return PARSE_FAIL;
	std::map<std::string, int> portCheck;
	for (size_t i = 0; i < _serverConfigs.size(); i++)
		portCheck[_serverConfigs[i].getServerPort()] += 1;
	for (std::map<std::string, int>::iterator it = portCheck.begin(); it != portCheck.end(); it++)
	{
		if (it->second > 1)
		{
			_log.debugLog("overlap port multi server");
			return PARSE_FAIL;
		}
	}
	return PARSE_OK;
}

std::vector<ServerConfig> Parser::getServerConfig(void) const
{
	return _serverConfigs;
}

int Parser::_getLocationInfo(std::stringstream &ss, std::string msg)
{
	ss >> _info;
	if (_info.back() != ';')
	{
		_log.debugLog(msg);
		return PARSE_FAIL;
	}
	_info.pop_back();
	return PARSE_OK;
}

bool Parser::_checkSemicolon(char c)
{
	return c == ';';
}

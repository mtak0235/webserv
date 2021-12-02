#include "Parser.hpp"

const std::string Parser::_keyInit = "server";
const std::string Parser::_keyServer[3] = {"listen", "server_name", "location"};
const std::string Parser::_keyLocation[8] = {"autoindex","client_max_body_size", "method", "index",
                                         "root", "cgi_extension", "cgi_path", "upload_folder"};

Parser::Parser(void)
    : _serverConfigs(std::vector<ServerConfig>())
{
}

Parser::~Parser()
{
}

void Parser::parse(const std::string& confFile)
{
    _ifs.open(confFile);

    while (!_ifs.eof())
    {
        _ifs >> _info;
        if (!_info.compare(_keyInit))
            _serverConfigs.push_back(_parseServerBlock());
    }

}

ServerConfig Parser::_parseServerBlock(void)
{
    ServerConfig sc;
    std::string tmp;

	_ifs >> _info;
	if (_info != "{")
		return sc;
    while (_info[0] != '}') {
		_ifs >> _info;
        if (!_info.compare(_keyServer[LISTEN]))
		{
			_ifs >> tmp;
			tmp = tmp.substr(0, tmp.find(";", 0));
            sc.setServerPort(tmp);   //serverPort;
		}
		else if (!_info.compare(_keyServer[SERVER_NAME]))
		{
			_ifs >> tmp;
            sc.setServerName(tmp);//serverName;
		}
		else if (!_info.compare(_keyServer[LOCATION]))
		{
			sc.setLocations(_parseLocationBlock());
			std::vector<LocationConfig> tmp = sc.getLocations();
			sc.setLocationsFind(tmp[tmp.size() - 1].getLocationName(), tmp[tmp.size() - 1]);
		}
    }
	_serverRemoveSemicolon(&sc);

    return sc;
}

LocationConfig Parser::_parseLocationBlock(void)
{
    LocationConfig lc;
	std::string tmp;
	lc.setCliBodySize(10);

	_ifs >> tmp >> _info;
	lc.setLocationName(tmp);
	if (_info != "{")
		return lc;
    while (_info[0] != '}') {
        _ifs >> _info;
        if (!_info.compare(_keyLocation[BODY_SIZE]))
		{
			_ifs >> tmp;
            lc.setCliBodySize(stoi(tmp));
		}
        else if (!_info.compare(_keyLocation[METHOD]))
		{
			std::string method;
 			while (1)
			{
				_ifs >> method;
				lc.setAllowMethod(method);
				if (method[method.length() - 1] == ';')
					break;
			}
		}
		else if (!_info.compare(_keyLocation[INDEX]))
        {
			std::string index;
			while (1)
			{
				_ifs >> index;
				lc.setIndexList(index);
				if (index[index.length() - 1] == ';')
					break;
			}
		}
        else if (!_info.compare(_keyLocation[ROOT]))
		{
			_ifs >> tmp;
			lc.setRoot(tmp);
		}
		else if (!_info.compare(_keyLocation[CGI_EXTENSION]))
		{
			_ifs >> tmp;
			lc.setCgiName(tmp);
		}
		else if (!_info.compare(_keyLocation[CGI_PATH]))
		{
			_ifs >> tmp;
			lc.setCgiPath(tmp);
		}
        else if (!_info.compare(_keyLocation[UPLOAD_FOLDER]))
		{
			_ifs >> tmp;
			lc.setUploadFolder(tmp);
		}
		else if(!_info.compare(_keyLocation[AUTO_INDEX]))
		{
			_ifs >> tmp;
			tmp = tmp.substr(0, 2);
			lc.setAutoIndex(tmp);
		}
    }
	_info = "";
	_locationRemoveSemicolon(&lc);
	return lc;
}

void Parser::_locationRemoveSemicolon(LocationConfig *lc)
{
	std::string tmpStr;
	std::vector<std::string> tmpVec;
	tmpStr = lc->getRoot();
	if (!tmpStr.empty())
	{
		tmpStr.pop_back();
		lc->setRoot(tmpStr);
	}
	tmpVec = lc->getIndexList();
	if (tmpVec.size() != 0)
	{
		tmpVec[tmpVec.size() - 1].pop_back();
		lc->popIndexList();
		lc->setIndexList(tmpVec[tmpVec.size() - 1]);
	}
	tmpVec = lc->getAllowMethod();
	if (tmpVec.size() != 0)
	{
		tmpVec[tmpVec.size() - 1].pop_back();
		lc->popAllowMethod();
		lc->setAllowMethod(tmpVec[tmpVec.size() - 1]);
	}
	tmpStr = lc->getCgiName();
	if (!tmpStr.empty())
	{
		tmpStr.pop_back();
		lc->setCgiName(tmpStr);
	}
	tmpStr = lc->getCgiPath();
	if (!tmpStr.empty())
	{
		tmpStr.pop_back();
		lc->setCgiPath(tmpStr);
	}
	tmpStr = lc->getUploadFolder();
	if (!tmpStr.empty())
	{
		tmpStr.pop_back();
		lc->setUploadFolder(tmpStr);
	}
}

void Parser::_serverRemoveSemicolon(ServerConfig *sc)
{
	std::string tmp;
	tmp = sc->getServerName();
	if (!tmp.empty())
	{
		tmp.pop_back();
		sc->setServerName(tmp);
	}
}

std::vector<ServerConfig> Parser::getServerConfig(void) const
{
	return _serverConfigs;
}

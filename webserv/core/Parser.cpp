#include "Parser.hpp"

const std::string Parser::_keyInit = "server";
const std::string Parser::_keyServer[3] = {"listen", "server_name", "location"};
const std::string Parser::_keyLocation[7] = {"client_max_body_size", "method", "index",
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
    
	_ifs >> _info;
	if (_info != "{")
		return sc;
    while (_info[0] != '}') {
		_ifs >> _info;
        if (!_info.compare(_keyServer[LISTEN]))
            _ifs >> sc.serverPort;
        else if (!_info.compare(_keyServer[SERVER_NAME]))
            _ifs >> sc.serverName;
		else if (!_info.compare(_keyServer[LOCATION]))
            sc.locations.push_back(_parseLocationBlock());
    }
	_serverRemoveSemicolon(&sc);
    return sc;
}

LocationConfig Parser::_parseLocationBlock(void)
{
    LocationConfig lc;
	lc.cliBodySize = 10;

	_ifs >> lc.locationName >> _info;
	if (_info != "{")
		return lc;
    while (_info[0] != '}') {
        _ifs >> _info;
        if (!_info.compare(_keyLocation[BODY_SIZE]))
            _ifs >> lc.cliBodySize;
        else if (!_info.compare(_keyLocation[METHOD]))
		{
			std::string method;
 			while (1)
			{
				_ifs >> method;
				lc.allowMethods.push_back(method);
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
				lc.indexList.push_back(index);
				if (index[index.length() - 1] == ';')
					break;
			}
		}
        else if (!_info.compare(_keyLocation[ROOT]))
            _ifs >> lc.root;
        else if (!_info.compare(_keyLocation[CGI_EXTENSION]))
            _ifs >> lc.cgiName;
        else if (!_info.compare(_keyLocation[CGI_PATH]))
            _ifs >> lc.cgiPath;
        else if (!_info.compare(_keyLocation[UPLOAD_FOLDER]))
            _ifs >> lc.uploadFolder;
    }
	_info = "";
	_locationRemoveSemicolon(&lc);
	return lc;
}

void Parser::_locationRemoveSemicolon(LocationConfig *lc)
{
	if (!lc->root.empty())
		lc->root.pop_back();
	if (lc->indexList.size() != 0)
		lc->indexList[lc->indexList.size() - 1].pop_back();
	if (lc->allowMethods.size() != 0)
		lc->allowMethods[lc->allowMethods.size() - 1].pop_back();
	if (!lc->cgiName.empty())
		lc->cgiName.pop_back();
	if (!lc->cgiPath.empty())
		lc->cgiPath.pop_back();
	if (!lc->uploadFolder.empty())
		lc->uploadFolder.pop_back();
}

void Parser::_serverRemoveSemicolon(ServerConfig *sc)
{
	if (!sc->serverName.empty())
		sc->serverName.pop_back();
}

std::vector<ServerConfig> Parser::getServerConfig(void) const
{
	return _serverConfigs;
}

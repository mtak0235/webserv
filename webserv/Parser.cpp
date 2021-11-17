#include "Parser.hpp"

static const std::string keyInit = "server";
static const std::string keyServer[3] = {"listen", "server_name", "location"};
static const std::string keyServer[7] = {"client_max_body_size", "method", "index",
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
        if (_info.compare(keyInit))
            _serverConfigs.push_back(_parseServerBlock());
    }

}

ServerConfig Parser::_parseServerBlock(void)
{
    ServerConfig sc;
    
    _ifs >> _info;
    while (_info[0] != '}') {
        if (_info.compare(keyServer[LISTEN]))
            _ifs >> sc.serverPort;
        else if (_info.compare(keyServer[SERVER_NAME]))
            _ifs >> sc.serverName;
        else if (_info.compare(keyServer[LOCATION]) == 0)
            sc.locations.push_back(_parseLocationBlock());
    }
    return sc;
}

LocationConfig Parser::_parseLocationBlock(void)
{
    LocationConfig lc;

    while (_info[0] != '}') {
        _ifs >> _info;
        if (_info.compare(_keyLocation[BODY_SIZE]))
            _ifs >> lc.cliBodySize;
        else if (_info.compare(_keyLocation[METHOD]))
            _ifs >> lc.allowMethod;
        else if (_info.compare(_keyLocation[INDEX]))
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
        else if (_info.compare(_keyLocation[ROOT]))
            _ifs >> lc.root;
        else if (_info.compare(_keyLocation[CGI_EXTENSION]))
            _ifs >> lc.cgiName;
        else if (_info.compare(_keyLocation[CGI_PATH]))
            _ifs >> lc.cgiPath;
        else if (_info.compare(_keyLocation[UPLOAD_FOLDER]))
            _ifs >> lc.uploadFolder;
    }
	return lc;
}

std::vector<ServerConfig> Parser::getServerConfig(void) const
{
	return _serverConfigs;
}

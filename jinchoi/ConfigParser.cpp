#include "ConfigParser.hpp"

ConfigParser::ConfigParser(void)
    : _servers(std::vector<Server>())
{
}

ConfigParser::~ConfigParser()
{
}

void ConfigParser::parse(void)
{
    Server s = _ParseServerConfig();
}

std::vector<ConfigParser::Server> ConfigParser::getServerConfig(void) const
{

}

ConfigParser::Server ConfigParser::_ParseServerConfig(void)
{
    /// locationl

}





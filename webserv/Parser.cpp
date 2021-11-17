#include "Config.hpp"

Config::Config(void)
    : _serverConfigs(std::vector<ServerConfig>())
{
}

Config::~Config()
{
}

void Config::parse(void)
{
    ServerConfig s = _ParseServerConfig();
}

std::vector<Config::ServerConfig> Config::getServerConfig(void) const
{
}

Config::ServerConfig Config::_ParseServerConfig(void)
{
}





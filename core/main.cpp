#include "Server.hpp"

int main(int argc, char *argv[])
{
  std::string configFile = Parser::getConfigFile(argc, argv);
  Server cluster;
	if (cluster.init(configFile) == NGX_FAIL)
		return NGX_FAIL;
	cluster.run();
	cluster.stop();
	return 0;
}

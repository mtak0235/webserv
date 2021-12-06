#include "Server.hpp"

int main(int argc, char *argv[])
{
  std::string configFile = Parser::getConfigFile(argc, argv);

  Cluster cluster(configFile);
	if (cluster.init() == FAIL)
		return FAIL;
	if (cluster.run() == FAIL) {
		cluster.closeAllConnection();
		return FAIL;
	}
	return SUCCESS;
}

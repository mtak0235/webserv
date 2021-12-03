
#include "Server.hpp"
#include <iostream>

int main(int ag, char **av)
{
	Server cluster;
	std::string conf = "";

	if (ag == 1)
		conf = "./basic.conf";
	else if (ag == 2)
		conf = av[1];
	if (conf.compare(""))
	{
		cluster.init(conf);
		cluster.run();
		cluster.stop();
	}
	return 0;
}

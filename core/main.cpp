
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
		
		if (cluster.init(conf) == NGX_FAIL)
			return NGX_FAIL;
		cluster.run();
		cluster.stop();
	}
	return 0;
}

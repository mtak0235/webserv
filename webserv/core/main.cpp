
#include "Cluster.hpp"
#include "ngxKqueue.hpp"
#include <iostream>

int main(int ag, char **av)
{
	ngxKqueue cluster;

	if (ag == 2 && av[1])
	{
		cluster.init(av[1]);
		cluster.run();
		cluster.stop();
	}
	return 0;
}

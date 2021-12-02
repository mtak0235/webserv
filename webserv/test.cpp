#include <unistd.h> // dup header file
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "./http/Response.hpp"

int main(void)
{
	Response res;

	std::string ret = res.generateAutoindexPage("./YoupiBanane/");
	std::cout << ret << std::endl;
	return (0);
}
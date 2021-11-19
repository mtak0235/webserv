#include "Server.hpp"
#include "cgi.hpp"

int main()
{
	Server s;
	cgi c;
	s.init("basic.conf");
	c.cgi_handler();
	return 0;
}

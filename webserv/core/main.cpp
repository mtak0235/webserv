#include "Server.hpp"
#include "cgi.hpp"
#include "log.hpp"

int main()
{
	Server s;
	cgi c;
	log l;

	l.debug_log("teststest");
	s.init("basic.conf");
	c.cgi_handler();
	return 0;
}

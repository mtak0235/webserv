#include "../http/Response.hpp"

#include <iostream>

int main()
{
	Response r;
	r.setHttpVersion("1.1");
	r.setStatusCode(200);
	r.setStatusMsg("OK");
	std::string res = r.makeResponse();
	std::cout << res << "\n";
	return 0;
}

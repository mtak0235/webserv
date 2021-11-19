#include "../http/Response.hpp"

#include <iostream>

int main()
{
	Response r;
	r.setStatusCode(200);
	r.setStatusMsg("OK");
	std::string body = "{\n}";
	std::string res = r.makeResponse(body);
	std::cout << res << "\n";
	return 0;
}

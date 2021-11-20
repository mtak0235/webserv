#include "../http/Response.hpp"

#include <iostream>

int main()
{
  //Server s;
  //s.init("basic.cof");
  
	Response r;
	r.setStatusCode(200);
	r.setStatusMsg("OK");
	r.setServerName("asdfasdf");
	std::string body = "{\n}";
	std::string res = r.makeResponse(body);
	std::cout << res << "\n";
	return 0;
}

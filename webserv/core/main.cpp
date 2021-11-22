#include "Response.hpp"
#include "Cluster.hpp"
#include "../http/Request.hpp"

#include <iostream>

int main(int ag, char **av)
{
	Cluster cluster;

	if (ag == 2 && av[1])
	{
		cluster.init(av[1]);
		cluster.run();
		cluster.stop();
	}
	// Response r;
	// r.setStatusCode(200);
	// r.setStatusMsg("OK");
	// r.setServerName("asdfasdf");
	// std::string body = "{\n}";
	// std::string res = r.makeResponse(body);
	// std::cout << res << "\n";

	const std::string test = "DELETE /index.html HTTP/1.1\nHost: localhost:4000\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\nUser-Agent: Mozilla/4.0\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nAccept-Charset: utf-8\nKeep-Alive: 300\nConnection: keep-alive\nContent-Length: 14242\n";
	Request	rq(test);
	// std::cout << rq.getMethod() << "\n";


	return 0;
}

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Config.hpp"
#include "Parser.hpp"
#include "Connection.hpp"
#include "core.hpp"
#include "ngxKqueue.hpp"

class Server : public ngxKqueue
{
	public :
		Server();
		~Server();

	private :
		Log _log;
		
};

#endif

#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>

class Log
{
public:
	Log();
	Log(Log& _x);
	~Log();
	Log operator=(Log& _x);
	void debugLog(std::string msg);
};

#endif





#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>

class log
{
public:
	log(/* args */);
	log(log& _x);
	~log();
	log operator=(log& _x);
	void debug_log(std::string msg);
};

#endif





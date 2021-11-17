#ifndef TIME_HPP
#define TIME_HPP

#include <iostream>
#include <string>

class time
{
private:
	int _msec;
	int _sec;
	std::string _day[7];
	std::string _month[12];
public:
	time();
	~time();
};

time::time()
{

}

time::~time()
{

}

#endif

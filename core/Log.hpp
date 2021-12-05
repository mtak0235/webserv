#ifndef LOG_HPP
# define LOG_HPP

#include <iostream>
#include <string>

class Log
{
  public:
    /* constructor & destructor */
    Log();
    Log(Log& _x); // 구현 빠짐
    ~Log();

    /* operator overloading */
    Log operator=(Log& _x); // 구현 빠짐

		/* public function */
    void debugLog(std::string msg);
};

#endif

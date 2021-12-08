#ifndef DEBUG_HPP
# define DEBUG_HPP

#include <iostream>
#include <sstream>
#include <string>

class Debug
{
  public:
    /* constructor & destructor */
    Debug();
    ~Debug();

		/* public static function */
    static void log(const std::string& msg);
		static const std::string getTime(void);
		static std::string FormatNum(int);
};

#endif

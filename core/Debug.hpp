#ifndef DEBUG_HPP
# define DEBUG_HPP

#include <iostream>
#include <string>

class Debug
{
  public:
    /* constructor & destructor */
    Debug();
    ~Debug();

		/* public static function */
    static void log(const std::string& msg);
};

#endif

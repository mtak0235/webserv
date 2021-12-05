#ifndef FORMATTER_HPP
# define FORMATTER_HPP

#include <string>
#include <sstream>
#include <ctime>

class Formatter
{
  public:
    /* constructor & destructor */
    Formatter();
    ~Formatter();

    /* public static function */
    static const std::string getDate(void);
    static const std::string makeFormat(const int& num);
    static const std::string ft_to_string(int n);
};

#endif

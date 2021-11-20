#include "log.hpp"

log::log()
{

}

log::~log()
{
    
}

void log::debug_log(std::string msg)
{
  std::cout << msg << '\n';
}
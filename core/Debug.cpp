#include "Debug.hpp"

Debug::Debug() { }

Debug::~Debug() { }

void Debug::log(const std::string& msg)
{
  std::cout << msg << '\n';
}

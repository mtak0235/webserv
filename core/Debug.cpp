#include "Debug.hpp"

Debug::Debug() { }

Debug::~Debug() { }

void Debug::log(const std::string& msg)
{
  std::cout << msg << '\n';
}

const std::string Debug::getTime(void)
{
	time_t timer = time(NULL);
	struct tm* t = localtime(&timer);
	std::string ret = "Log time [";
	ret += FormatNum(t->tm_hour);
	ret += " : ";
	ret += FormatNum(t->tm_min);
	ret += " : ";
	ret += FormatNum(t->tm_sec);
	ret += "]\n";
	return ret;
}

std::string Debug::FormatNum(int num)
{
	std::string ret = "";
  if (num < 10) ret += "0";
  std::stringstream ss;
  ss << num;
  std::string str;
  ss >> str;
  ret += str;
  return ret;
}

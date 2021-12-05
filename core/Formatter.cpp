#include "Formatter.hpp"

Formatter::Formatter() { }

Formatter::~Formatter() { }

const std::string Formatter::getDate(void)
{
  std::string ret = "";
  time_t timer = time(NULL);
  struct tm* t = localtime(&timer);
  const std::string wday[7] = {"Sun", "Mon", "Tue", "Wed", "Thur",
                               "Fri", "Sat"};
  ret += wday[t->tm_wday] + ", ";
  ret += makeFormat(t->tm_mday) + " ";
  const std::string wmon[12] = {"Jan", "Feb", "Mar", "Apr", "May",
                                "Jun", "Jul", "Aug", "Sep", "Oct",
                                "Nov", "Dec"};
  ret += (wmon[t->tm_mon] + " ");
  ret += makeFormat(t->tm_year + 1900) + " ";
  ret += makeFormat(t->tm_hour) + ":";
  ret += makeFormat(t->tm_min) + ":";
  ret += makeFormat(t->tm_sec) + " GMT\n";
  return ret;
}

const std::string Formatter::makeFormat(const int& num)
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

const std::string Formatter::ft_to_string(int n)
{
  bool isNeg = false;
  if (n < 0)
  {
    n *= -1;
    isNeg = true;
  }
  int digit = 1, decimal = 1, temp = n;
  while (temp /= 10)
  {
    digit++;
    decimal *= 10;
  }
  std::string ret;
  if (isNeg) ret += '-';
  for (int d = 0; d < digit; d++)
  {
    ret += n / decimal + '0';
    n %= decimal;
    decimal /= 10;
  }
  return ret;
}

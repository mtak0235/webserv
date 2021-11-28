#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void _parseCgiResponse(std::string str)
{
	std::stringstream ss;
	ss << str;
	std::vector<std::string> v;
	while (!ss.eof())
	{
		std::string l;
		std::getline(ss, l);
		v.push_back(l);
	}
	std::cout << v[0] << std::endl;
	std::cout << v[1] << std::endl;
	if (v[2] == "")
		std::cout << v[2] << std::endl;
	std::cout << v[3] << std::endl;
	
}

int main()
{
	_parseCgiResponse("11 11\n2222\n\n333");

}
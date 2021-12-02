#include <iostream>
#include <fstream>

int main()
{
	std::ofstream ofs;
	ofs.open("./aa.txt");

	ofs.close();
	return 0;
}
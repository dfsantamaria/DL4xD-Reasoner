#include "stdafx.h" // only on windows
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

std::fstream out;

int main()
{
	out.open("Example/generator.txt", std::fstream::out);
	for (int i = 1; i <= 50; i++)
		out << "V0{ind" << i << "}" << std::endl;
	out.close();
}
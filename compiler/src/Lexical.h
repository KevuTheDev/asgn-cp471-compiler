#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Lexical
{
public:
	Lexical();
	Lexical(const std::string &filename);

private:
	std::ifstream is;
	char peek;
	uint32_t line;
	

	bool scan(); // bool: true- continue, false- eof/exit

	void readchar();
	bool readchar(char c); // this does the peaking for specific grammar
};


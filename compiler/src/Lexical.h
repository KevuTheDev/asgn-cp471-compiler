#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"


class Lexical
{
public:
	Lexical();
	Lexical(const std::string &filename);

private:
	std::ifstream is;
	char peek;
	uint32_t line;
	

	// Utilities
	bool checkExtension(const std::string& filepath);


	bool nextToken(); // bool: true- continue, false- eof/exit

	void readChar();
	bool readChar(char c); // this does the peaking for specific grammar
};


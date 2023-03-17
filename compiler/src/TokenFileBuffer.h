#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"

class TokenFileBuffer
{
public:
	TokenFileBuffer();
	TokenFileBuffer(const std::string &filename);

	void append(const std::string& value);
	void append(const char* value);


private:
	std::ofstream _os;
	std::string _fileName;


	std::string _buffer = ""; // buffer size 2048
	uint16_t _bufferSize = 2048;

	// Utilities
	bool checkExtension(const std::string& filepath);
};


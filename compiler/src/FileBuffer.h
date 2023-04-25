#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "globals.h"

class FileBuffer
{
public:
	FileBuffer(const std::string &filename);
	FileBuffer();
	virtual ~FileBuffer();

	void writeToFile(const std::string& value);
	void writeToFile(const char* value);


private:
	// Utilities
	virtual bool checkExtension(const std::string& filepath);
	void close();

private:
	std::ofstream _os;
	std::string _fileName;

};


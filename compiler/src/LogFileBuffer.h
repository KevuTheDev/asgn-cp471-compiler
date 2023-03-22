#pragma once
#include <iostream>
#include <string>
#include "FileBuffer.h"

class LogFileBuffer : public FileBuffer
{
public:
	using FileBuffer::FileBuffer;

	bool checkExtension(const std::string& filepath);
	void errorChar(int linenumber, int rownumber, const std::string& errorchar);
};


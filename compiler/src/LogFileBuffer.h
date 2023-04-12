#pragma once
#include <iostream>
#include <string>
#include "FileBuffer.h"

class LogFileBuffer : public FileBuffer
{
public:
	using FileBuffer::FileBuffer;
	~LogFileBuffer();

	bool checkExtension(const std::string& filepath);
	void logLexicalError(int linenumber, int rownumber, const std::string& errorchar);
	void logSyntaxError(int linenumber, int rownumber, const std::string& errorchar);
};


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
	void logLexicalError(uint32_t linenumber, uint32_t rownumber, const std::string& errorchar);
	void logLexicalErrorNew(uint32_t linenumber, uint32_t rownumber, const std::string& errorchar, const std::string& currentLine);
	void logSyntaxError(uint32_t linenumber, uint32_t rownumber, const std::string& errorchar);
};


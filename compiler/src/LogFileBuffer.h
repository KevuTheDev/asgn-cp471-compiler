#pragma once
#include "FileBuffer.h"

class LogFileBuffer : public FileBuffer
{
public:
	using FileBuffer::FileBuffer;

	bool checkExtension(const std::string& filepath);
	void logLexicalError(uint32_t linenumb, uint32_t charposition, const std::string& error, const std::string& currentline);
	void logSyntaxError(uint32_t linenumber, uint32_t charposition, const std::string& error);
};


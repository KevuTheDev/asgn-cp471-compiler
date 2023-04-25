#pragma once
#include "FileBuffer.h"

class LogFileBuffer : public FileBuffer
{
public:
	using FileBuffer::FileBuffer;
	using FileBuffer::~FileBuffer;

	bool checkExtension(const std::string& filepath);
	void logLexicalError(uint64_t linenumb, uint64_t charposition, const std::string& error, const std::string& currentline);
	void logSyntaxError(uint64_t linenumber, uint64_t charposition, const std::string& error);
};


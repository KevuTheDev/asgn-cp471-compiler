#pragma once
#include "FileBuffer.h"

class TokenFileBuffer: public FileBuffer
{
public:
	using FileBuffer::FileBuffer;

	bool checkExtension(const std::string& filepath);
};


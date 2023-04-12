#pragma once
#include <iostream>
#include "FileBuffer.h"

class TokenFileBuffer: public FileBuffer
{
public:
	using FileBuffer::FileBuffer;
	~TokenFileBuffer();

	bool checkExtension(const std::string& filepath);
};


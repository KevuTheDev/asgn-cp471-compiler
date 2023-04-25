#pragma once

#include "FileBuffer.h"

class SymbolTableFileBuffer : public FileBuffer
{
public:
	using FileBuffer::FileBuffer;

	bool checkExtension(const std::string& filepath);

private:

};


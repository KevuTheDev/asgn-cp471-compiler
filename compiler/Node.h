#pragma once
#include <stdint.h>
#include <string>

class Node
{
public:
	Node();
	~Node();

private:
	uint32_t lineNumber;
	uint32_t charPosition;
	std::string lexeme;
};


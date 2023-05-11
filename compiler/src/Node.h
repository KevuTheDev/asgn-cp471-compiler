#pragma once
#include <stdint.h>
#include <string>

class Node
{
public:
	Node(uint32_t linenum, uint32_t charpos, std::string lex);
	~Node();

	uint32_t getLineNumber();
	uint32_t getCharPosition();
	std::string getLexeme();

private:
	uint32_t _lineNumber;
	uint32_t _charPosition;
	std::string _lexeme;
};


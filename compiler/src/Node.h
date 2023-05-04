#pragma once
#include <cstdint>
#include <string>

class Node
{
public:
	Node(uint32_t linenum, uint32_t charpos, std::string lexeme);
	~Node();

	uint32_t getLineNum();
	uint32_t getCharPos();
	std::string getLexeme();

private:
	uint32_t _lineNum;
	uint32_t _charPos;
	std::string _lexeme;
};


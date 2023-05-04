#pragma once
#include <format>
#include <memory>
#include <vector>

#include "TokenFileBuffer.h"
#include "TokenNode.h"

class TokenList
{
public:
	TokenList();
	~TokenList();

	void linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer);

	void append(uint32_t linenum, uint32_t charpos, std::string lexeme, compiler::TOKEN token);

	uint32_t getLineNumber(uint32_t index);
	uint32_t getCharPosition(uint32_t index);
	compiler::TOKEN getToken(uint32_t index);
	std::string getLexeme(uint32_t index);

	uint32_t getSize();
	
	void print();

private:

private:
	std::shared_ptr<TokenFileBuffer> _tokenFileBuffer;

	std::vector<TokenNode> _tokenList;

	uint32_t _printLineNumLimit;
	uint32_t _printCharPosLimit;
	uint32_t _printTokenLimit;
	uint32_t _printLexemeLimit;
};


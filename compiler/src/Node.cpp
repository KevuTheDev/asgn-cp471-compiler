#include "Node.h"

Node::Node(uint32_t linenum, uint32_t charpos, std::string lex)
{
	this->_lineNumber = linenum;
	this->_charPosition = charpos;
	this->_lexeme = lex;
}

Node::~Node()
{
}

uint32_t Node::getLineNumber()
{
	return this->_lineNumber;
}

uint32_t Node::getCharPosition()
{
	return this->_charPosition;
}

std::string Node::getLexeme()
{
	return this->_lexeme;
}

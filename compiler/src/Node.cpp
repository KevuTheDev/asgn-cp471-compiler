#include "Node.h"

Node::Node(uint32_t linenum, uint32_t charpos, std::string lexeme)
{
    this->_lineNum = linenum;
    this->_charPos = charpos;
    this->_lexeme = lexeme;
}

Node::~Node()
{
}

uint32_t Node::getLineNum()
{
    return this->_lineNum;
}

uint32_t Node::getCharPos()
{
    return this->_charPos;
}

std::string Node::getLexeme()
{
    return this->_lexeme;
}

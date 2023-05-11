#pragma once
#include "Node.h"
#include "globals.h"

class TokenNode: public Node
{
public:
	TokenNode(uint32_t linenum, uint32_t charpos, std::string lex, compiler::TOKEN token) : Node(linenum, charpos, lex), _token(token) {};
	~TokenNode();

	compiler::TOKEN getToken();

private:
	compiler::TOKEN _token;
};


#pragma once

#include "Node.h"
#include "globals.h"

class TokenNode : public Node
{
public:
	TokenNode(uint32_t linenum, uint32_t charpos, std::string lexeme, compiler::TOKEN token) : Node(linenum, charpos, lexeme), _token(token) {};
	~TokenNode();

	compiler::TOKEN getToken();

private:
	compiler::TOKEN _token;
};


#pragma once
#include <memory>
#include <string>
#include <vector>


#include "globals.h"

class SyntaxNode
{
public:
	SyntaxNode(bool isTerminalNode, std::string value);
	~SyntaxNode();

	void append(std::shared_ptr<SyntaxNode> node);
	void removeLastNode();

	bool hasNoChildren();

	bool isTerminalNode();

private:

private:
	std::vector<std::shared_ptr<SyntaxNode>> _children;

	bool _isTerminalNode; // non-terminal = false or terminal = true

	std::string _nonTerminalValue;
	std::string _lexeme;
	compiler::TOKEN _token;

	// maybe type, or scope inserted for semantic analysis

};


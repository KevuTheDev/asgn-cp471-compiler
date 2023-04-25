#pragma once

#include "SyntaxNode.h"

class SyntaxTree
{
public:
	SyntaxTree();
	~SyntaxTree();

	void connectNodeToHead(std::shared_ptr<SyntaxNode> node);

	void print();

private:

private:
	std::shared_ptr<SyntaxNode> _head;

};


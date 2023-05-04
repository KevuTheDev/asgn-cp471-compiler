#pragma once

#include "SyntaxNode.h"

class SyntaxTree
{
public:
	SyntaxTree();
	~SyntaxTree();

	void connectNodeToHead(std::shared_ptr<SyntaxNode> node);

	void print();

	void print_aux(std::shared_ptr<SyntaxNode> node, uint16_t count);

private:

private:
	std::shared_ptr<SyntaxNode> _head;

};


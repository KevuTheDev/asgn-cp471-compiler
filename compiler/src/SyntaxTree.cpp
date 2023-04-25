#include "SyntaxTree.h"

SyntaxTree::SyntaxTree()
{
}

SyntaxTree::~SyntaxTree()
{
}

void SyntaxTree::connectNodeToHead(std::shared_ptr<SyntaxNode> node)
{
	this->_head = node;
}

void SyntaxTree::print()
{
}

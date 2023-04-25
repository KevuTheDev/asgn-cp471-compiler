#include "SyntaxNode.h"

SyntaxNode::SyntaxNode(bool isterminalnode, std::string value)
{
	this->_isTerminalNode = isterminalnode;
	
	if (this->_isTerminalNode) {
		this->_lexeme = value;
	}
	else {
		this->_nonTerminalValue = value;
	}
}

SyntaxNode::~SyntaxNode()
{
}

void SyntaxNode::append(std::shared_ptr<SyntaxNode> node)
{
	this->_children.push_back(node);
}

void SyntaxNode::removeLastNode()
{
	if (!hasNoChildren()) {
		this->_children.pop_back();
	}
}

bool SyntaxNode::hasNoChildren()
{
	return this->_children.size() == 0;
}

bool SyntaxNode::isTerminalNode()
{
	return this->_isTerminalNode;
}

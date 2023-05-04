#include "SyntaxNode.h"

SyntaxNode::SyntaxNode(std::string category)
{
	this->_category = category;
	this->_data = "";
	this->_type = "";
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

std::vector<std::shared_ptr<SyntaxNode>> SyntaxNode::getChildren()
{
	return this->_children;
}

bool SyntaxNode::isTerminalNode()
{
	return this->_data.size() != 0;
}

void SyntaxNode::setData(std::string data)
{
	this->_data = data;
}

std::string SyntaxNode::getData()
{
	return this->_data;
}

std::string SyntaxNode::getCategory()
{
	return this->_category;
}

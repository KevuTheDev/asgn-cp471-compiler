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
	auto count = 0;
	std::cout << this->_head->getScope() << std::endl;
	print_aux(this->_head, count);
}

void SyntaxTree::print_aux(std::shared_ptr<SyntaxNode> node, uint16_t count)
{
	count++;
	for (auto i : node->getChildren()) {
		if (i->isTerminalNode()) {
			std::cout << std::string((count - 1)*2, ' ') << "|--> [" << i->getData() << "]" << std::endl;
		}
		else {
			std::cout << std::string((count - 1) * 2, ' ') << "|--> " << i->getScope() << std::endl;
		}
		print_aux(i, count);
	}
}


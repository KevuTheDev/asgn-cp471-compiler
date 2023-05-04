#pragma once
#include <memory>
#include <string>
#include <vector>


#include "globals.h"

class SyntaxNode
{
public:
	SyntaxNode(std::string category);
	~SyntaxNode();

	void append(std::shared_ptr<SyntaxNode> node);
	void removeLastNode();

	bool hasNoChildren();
	std::vector<std::shared_ptr<SyntaxNode>> getChildren();

	bool isTerminalNode();


	void setData(std::string data);
	std::string getData();

	std::string getCategory();


private:

private:
	std::vector<std::shared_ptr<SyntaxNode>> _children;

	std::string _category;

	std::string _data; // empty is non-terminal, non-empty is terminal

	
	std::string _type;


	// maybe type, or scope inserted for semantic analysis

};


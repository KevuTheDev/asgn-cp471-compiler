#pragma once
#include <memory>

#include "LogFileBuffer.h"
#include "TokenList.h"
#include "SyntaxTree.h"

class Syntax
{
public:
	Syntax();
	~Syntax();

	void linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer);
	void linkTokenList(std::shared_ptr<TokenList> list);
	void linkSyntaxTree(std::shared_ptr<SyntaxTree> tree);

	void run();

	bool getError();

private:

private:
	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<TokenList> _tokenList;
	std::shared_ptr<SyntaxTree> _syntaxTree;

	bool _error;
};

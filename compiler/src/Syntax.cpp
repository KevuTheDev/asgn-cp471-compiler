#include "Syntax.h"

Syntax::Syntax()
{
	this->_error = false;
}

Syntax::~Syntax()
{
}

void Syntax::linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer)
{
	this->_logFileBuffer = buffer;
}

void Syntax::linkTokenList(std::shared_ptr<TokenList> list)
{
	this->_tokenList = list;
}

void Syntax::linkSyntaxTree(std::shared_ptr<SyntaxTree> tree)
{
	this->_syntaxTree = tree;
}

void Syntax::run()
{
}

bool Syntax::getError()
{
	return false;
}

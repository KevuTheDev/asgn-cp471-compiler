#include "Semantic.h"

Semantic::Semantic()
{
	this->_error = false;
}

Semantic::~Semantic()
{
}

void Semantic::linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer)
{
	this->_logFileBuffer = buffer;
}

void Semantic::linkSyntaxTree(std::shared_ptr<SyntaxTree> tree)
{
	this->_syntaxTree = tree;
}

void Semantic::linkSymbolTable(std::shared_ptr<SymbolTable> table)
{
	this->_symbolTable = table;
}

void Semantic::run()
{
}

bool Semantic::getError()
{
	return false;
}

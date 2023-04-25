#pragma once
#include <memory>

#include "LogFileBuffer.h"
#include "SyntaxTree.h"
#include "SymbolTable.h"

class Semantic
{
public:
	Semantic();
	~Semantic();

	void linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer);
	void linkSyntaxTree(std::shared_ptr<SyntaxTree> tree);
	void linkSymbolTable(std::shared_ptr<SymbolTable> table);

	void run();

	bool getError();
private:


private:
	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<SyntaxTree> _syntaxTree;
	std::shared_ptr<SymbolTable> _symbolTable;

	bool _error;

};


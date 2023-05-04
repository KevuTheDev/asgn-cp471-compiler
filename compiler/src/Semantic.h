#pragma once
#include <memory>

#include "LogFileBuffer.h"
#include "TokenList.h"
#include "SyntaxTree.h"
#include "SymbolTableManager.h"
#include <stack>

class Semantic
{
public:
	Semantic();
	~Semantic();

	void linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer);
	void linkTokenList(std::shared_ptr<TokenList> list);
	void linkSyntaxTree(std::shared_ptr<SyntaxTree> tree);
	void linkSymbolTableManager(std::shared_ptr<SymbolTableManager> table);

	void run();

	bool getError();
private:

	void pushCategoryStack(std::string category);
	void popCategoryStack();
	std::string getTopCategory();


	void pushScopeStack(std::string scope);
	void popScopeStack();
	std::string getTopScope();

	void start();
	void start_aux(std::shared_ptr<SyntaxNode> node);

private:
	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<TokenList> _tokenList;
	std::shared_ptr<SyntaxTree> _syntaxTree;
	std::shared_ptr<SymbolTableManager> _symbolTableManager;

	bool _error;

	std::stack<std::string> _categoryStack;
	std::stack<std::string> _scopeStack;


	std::shared_ptr<SymbolTable::SymbolTableRow> _myRow;


};

/*
	Category Stack
	methods:
	- start()	      : global
	- FDEC()	      : function
	- PARAMS()	      : param
	- DECL()		  : decl
	- STATEMENT()	  : if, print, return, while
	- STATEMENT_EXT() : else
	- FACTOR()		  : factor

*/
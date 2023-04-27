#pragma once

#include "globals.h"
#include "TokenList.h"
#include "SymbolTableManager.h"

#include "LogFileBuffer.h"
#include "TokenList.h"
#include "SyntaxTree.h"
#include <stack>

class TypeCheck
{
public:
	struct SNode {
		// lexeme, type, scope, line, char, category, function params
		uint32_t lineNum;
		uint32_t charPos;
		compiler::TOKEN token;
		std::string lexeme;
		std::string scope;
		std::string type;
		std::string category;
	};

	TypeCheck();
	~TypeCheck();

	void linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer);
	void linkTokenList(std::shared_ptr<TokenList> list);
	void linkSyntaxTree(std::shared_ptr<SyntaxTree> tree);
	void linkSymbolTableManager(std::shared_ptr<SymbolTableManager> table);

	void run();

	bool getError();
private:

	enum CATEGORY {
		GLOBAL,
		FUNCTION,
		PARAM,
		IF,
		PRINT,
		RETURN,
		WHILE
	};


	compiler::TOKEN getPeek();
	void getNextToken();
	bool matchToken(compiler::TOKEN token);

	void pushCategoryStack(std::string category);
	void popCategoryStackTop();


	// Recursive Decent Functions
	void start();

	bool PROGRAM();
	bool FDECLS();
	bool FDECLS_EXT();
	bool FDEC();
	bool PARAMS();
	bool PARAMS_EXT();
	bool FNAME();
	bool DECLARATIONS();
	bool DECLARATIONS_EXT();
	bool DECL();
	bool TYPE();
	bool VARLIST();
	bool VARLIST_EXT();
	bool STATEMESEQ();
	bool STATEMESEQ_EXT();
	bool STATEMENT();
	bool STATEMENT_EXT();
	bool EXPR();
	bool EXPR_EXT();
	bool TERM();
	bool TERM_EXT();
	bool FACTOR();
	bool FACTOR_EXT();
	bool EXPRSEQ();
	bool EXPRSEQ_EXT();
	bool BEXPR();
	bool BEXPR_EXT();
	bool BTERM();
	bool BTERM_EXT();
	bool BFACTOR();
	bool BFACTOR_EXT();
	bool COMP();
	bool VAR();
	bool VAR_EXT();
	bool NUMBER();


private:
	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<TokenList> _tokenList;
	std::shared_ptr<SyntaxTree> _syntaxTree;
	std::shared_ptr<SymbolTableManager> _symbolTableManager;

	bool _error;

	compiler::TOKEN _currentToken;
	uint32_t _tokenListIndex;  // this->_position
	uint32_t _tokenListLimit;  // this->_limit

	std::stack<std::string> _categoryStack;


	std::shared_ptr<SymbolTable::SymbolTableRow> _myRow;

	std::vector<std::string> queue;

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
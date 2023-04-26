#pragma once
#include <memory>

#include "LogFileBuffer.h"
#include "TokenList.h"
#include "SyntaxTree.h"
#include "SymbolTable.h"
#include <stack>

class Semantic
{
public:
	Semantic();
	~Semantic();

	void linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer);
	void linkTokenList(std::shared_ptr<TokenList> list);
	void linkSyntaxTree(std::shared_ptr<SyntaxTree> tree);
	void linkSymbolTable(std::shared_ptr<SymbolTable> table);

	void run();

	bool getError();
private:
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

	bool isIdentifierInTable(std::string id);


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
	std::shared_ptr<SymbolTable> _symbolTable;

	bool _error;

	compiler::TOKEN _currentToken;
	uint32_t _tokenListIndex;  // this->_position
	uint32_t _tokenListLimit;  // this->_limit

	std::stack<std::string> _categoryStack;
	std::stack<std::string> _scopeStack;

	std::vector<std::shared_ptr<SNode>> _nodes;

	std::shared_ptr<SNode> myNode;

};


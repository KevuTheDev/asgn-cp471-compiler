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
	compiler::TOKEN getPeek();
	void getNextToken();
	bool matchToken(compiler::TOKEN token, std::shared_ptr<SyntaxNode> node);

	// Error Handling
	void reportError(const std::string& errorchar);

	std::shared_ptr<SyntaxNode> createAndConnectNode(std::shared_ptr<SyntaxNode> dst, std::string scope);


	// Recursive Decent Functions
	void start();

	bool PROGRAM(std::shared_ptr<SyntaxNode> node);
	bool FDECLS(std::shared_ptr<SyntaxNode> node);
	bool FDECLS_EXT(std::shared_ptr<SyntaxNode> node);
	bool FDEC(std::shared_ptr<SyntaxNode> node);
	bool PARAMS(std::shared_ptr<SyntaxNode> node);
	bool PARAMS_EXT(std::shared_ptr<SyntaxNode> node);
	bool FNAME(std::shared_ptr<SyntaxNode> node);
	bool DECLARATIONS(std::shared_ptr<SyntaxNode> node);
	bool DECLARATIONS_EXT(std::shared_ptr<SyntaxNode> node);
	bool DECL(std::shared_ptr<SyntaxNode> node);
	bool TYPE(std::shared_ptr<SyntaxNode> node);
	bool VARLIST(std::shared_ptr<SyntaxNode> node);
	bool VARLIST_EXT(std::shared_ptr<SyntaxNode> node);
	bool STATEMENTSEQ(std::shared_ptr<SyntaxNode> node);
	bool STATEMENTSEQ_EXT(std::shared_ptr<SyntaxNode> node);
	bool STATEMENT(std::shared_ptr<SyntaxNode> node);
	bool STATEMENT_EXT(std::shared_ptr<SyntaxNode> node);
	bool EXPR(std::shared_ptr<SyntaxNode> node);
	bool EXPR_EXT(std::shared_ptr<SyntaxNode> node);
	bool TERM(std::shared_ptr<SyntaxNode> node);
	bool TERM_EXT(std::shared_ptr<SyntaxNode> node);
	bool FACTOR(std::shared_ptr<SyntaxNode> node);
	bool FACTOR_EXT(std::shared_ptr<SyntaxNode> node);
	bool EXPRSEQ(std::shared_ptr<SyntaxNode> node);
	bool EXPRSEQ_EXT(std::shared_ptr<SyntaxNode> node);
	bool BEXPR(std::shared_ptr<SyntaxNode> node);
	bool BEXPR_EXT(std::shared_ptr<SyntaxNode> node);
	bool BTERM(std::shared_ptr<SyntaxNode> node);
	bool BTERM_EXT(std::shared_ptr<SyntaxNode> node);
	bool BFACTOR(std::shared_ptr<SyntaxNode> node);
	bool BFACTOR_EXT(std::shared_ptr<SyntaxNode> node);
	bool COMP(std::shared_ptr<SyntaxNode> node);
	bool VAR(std::shared_ptr<SyntaxNode> node);
	bool VAR_EXT(std::shared_ptr<SyntaxNode> node);
	bool NUMBER(std::shared_ptr<SyntaxNode> node);

private:
	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<TokenList> _tokenList;
	std::shared_ptr<SyntaxTree> _syntaxTree;

	bool _error;

	compiler::TOKEN _currentToken;
	uint32_t _tokenListIndex;  // this->_position
	uint32_t _tokenListLimit;  // this->_limit


};

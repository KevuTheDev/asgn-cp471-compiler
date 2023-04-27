#include "TypeCheck.h"

TypeCheck::TypeCheck()
{
	this->_error = false;

	this->_tokenListIndex = 0;
	this->_tokenListLimit = 0;
	this->_categoryStack = std::stack<std::string>();

}

TypeCheck::~TypeCheck()
{
}

void TypeCheck::linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer)
{
	this->_logFileBuffer = buffer;
}

void TypeCheck::linkTokenList(std::shared_ptr<TokenList> list)
{
	this->_tokenList = list;
	this->_tokenListLimit = this->_tokenList->getSize();
	this->_currentToken = this->_tokenList->getToken(this->_tokenListIndex);
}

void TypeCheck::linkSyntaxTree(std::shared_ptr<SyntaxTree> tree)
{
	this->_syntaxTree = tree;
}

void TypeCheck::linkSymbolTableManager(std::shared_ptr<SymbolTableManager> table)
{
	this->_symbolTableManager = table;
}

void TypeCheck::run()
{
	this->start();
}

bool TypeCheck::getError()
{
	return this->_error;
}

compiler::TOKEN TypeCheck::getPeek()
{
	return this->_currentToken;
}

void TypeCheck::getNextToken()
{
	this->_tokenListIndex += 1;

	if (this->_tokenListIndex == this->_tokenListLimit) {
		this->_currentToken = compiler::DOLLAR_SIGN;
	}
	else {
		this->_currentToken = this->_tokenList->getToken(this->_tokenListIndex);
	}
}

bool TypeCheck::matchToken(compiler::TOKEN token)
{
	if (getPeek() != token) {
		return false;
	}

	if (this->_categoryStack.size() != 0) {
		if (this->_categoryStack.top() == "factor") { // EXPR
			std::string id = this->_tokenList->getLexeme(this->_tokenListIndex);
			queue.push_back(id);
		}
		else if (this->_categoryStack.top() == "comp") {
			std::string id = this->_tokenList->getLexeme(this->_tokenListIndex);
			queue.push_back(id);
		}
		else if (this->_categoryStack.top() == "term_ext") {
			std::string id = this->_tokenList->getLexeme(this->_tokenListIndex);
			queue.push_back(id);
		}
		else if (this->_categoryStack.top() == "expr_ext") {
			std::string id = this->_tokenList->getLexeme(this->_tokenListIndex);
			queue.push_back(id);
		}
		else if (this->_categoryStack.top() == "number") {
			std::string id = this->_tokenList->getLexeme(this->_tokenListIndex);
			queue.push_back(id);
		}
	}


	getNextToken();
	return true;
}

void TypeCheck::pushCategoryStack(std::string category)
{
	this->_categoryStack.push(category);
}

void TypeCheck::popCategoryStackTop()
{
	this->_categoryStack.pop();
	if (this->_categoryStack.size() == 0) {
		std::cout << "==================POP=================" << std::endl;
		for (auto i : queue) {
			std::cout << i << std::endl;
		}
		queue.clear();
	}
}

void TypeCheck::start()
{
	std::shared_ptr<SyntaxNode> node = std::make_shared<SyntaxNode>(false, "PROGRAM");
	this->_syntaxTree->connectNodeToHead(node);

	bool hmm = PROGRAM();

	if (!hmm) {
		this->_error = true;
		std::cout << "INCOMPLETE" << std::endl;
		return;
	}

	std::cout << "COMPLETE" << std::endl;

}

bool TypeCheck::PROGRAM()
{
	if (getPeek() == compiler::DOLLAR_SIGN || getPeek() == compiler::DOT
		|| getPeek() == compiler::SEMICOLON || getPeek() == compiler::ID
		|| getPeek() == compiler::KW_DEF || getPeek() == compiler::KW_DOUBLE
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_INT
		|| getPeek() == compiler::KW_PRINT || getPeek() == compiler::KW_RETURN
		|| getPeek() == compiler::KW_WHILE) {
		return FDECLS() && DECLARATIONS()
			&& STATEMESEQ() && matchToken(compiler::DOT);
	}

	//reportError("PROGRAM");
	return false;
}

bool TypeCheck::FDECLS()
{
	if (getPeek() == compiler::KW_DEF) {
		return FDEC() && matchToken(compiler::SEMICOLON)
			&& FDECLS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_DEF
		|| getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_IF
		|| getPeek() == compiler::KW_INT || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return FDECLS_EXT();
	}

	//reportError("FDECLS");
	return false;
}

bool TypeCheck::FDECLS_EXT()
{
	if (getPeek() == compiler::KW_DEF) {
		return FDEC() && matchToken(compiler::SEMICOLON) && FDECLS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_DOUBLE
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_INT
		|| getPeek() == compiler::KW_PRINT || getPeek() == compiler::KW_RETURN
		|| getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return true;
	}

	//reportError("FDECLS_EXT");
	return false;
}

bool TypeCheck::FDEC()
{
	if (getPeek() == compiler::KW_DEF) {
		return matchToken(compiler::KW_DEF) && TYPE()
			&& FNAME() && matchToken(compiler::LEFT_PAREN)
			&& PARAMS() && matchToken(compiler::RIGHT_PAREN)
			&& DECLARATIONS() && STATEMESEQ()
			&& matchToken(compiler::KW_FED);
	}

	//reportError("FDEC");
	return false;
}

bool TypeCheck::PARAMS()
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		return TYPE() && VAR()
			&& PARAMS_EXT();
	}

	//reportError("PARAMS");
	return false;
}

bool TypeCheck::PARAMS_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchToken(compiler::COMMA) && PARAMS();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		return true;
	}

	//reportError("PARAMS_EXT");
	return false;
}

bool TypeCheck::FNAME()
{
	if (getPeek() == compiler::ID) {
		return matchToken(compiler::ID);
	}

	//reportError("FNAME");
	return false;
}

bool TypeCheck::DECLARATIONS()
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		return DECL() && matchToken(compiler::SEMICOLON)
			&& DECLARATIONS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return DECLARATIONS_EXT();
	}

	//reportError("DECLARATIONS");
	return false;
}

bool TypeCheck::DECLARATIONS_EXT()
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		return DECL() && matchToken(compiler::SEMICOLON)
			&& DECLARATIONS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return true;
	}

	//reportError("DECLARATIONS_EXT");
	return false;
}

bool TypeCheck::DECL()
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		return TYPE() && VARLIST();
	}

	//reportError("DECL");
	return false;
}

bool TypeCheck::TYPE()
{
	if (getPeek() == compiler::KW_DOUBLE) {
		return matchToken(compiler::KW_DOUBLE);
	}
	else if (getPeek() == compiler::KW_INT) {
		return matchToken(compiler::KW_INT);
	}

	//reportError("TYPE");
	return false;
}

bool TypeCheck::VARLIST()
{
	if (getPeek() == compiler::ID) {
		return VAR() && VARLIST_EXT();
	}

	//reportError("VARLIST");
	return false;
}

bool TypeCheck::VARLIST_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchToken(compiler::COMMA) && VARLIST();
	}
	else if (getPeek() == compiler::SEMICOLON) {
		// EPSILON
		return true;
	}

	//reportError("VARLIST_EXT");
	return false;
}

bool TypeCheck::STATEMESEQ()
{
	if (getPeek() == compiler::SEMICOLON || getPeek() == compiler::ID
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		return STATEMENT() && STATEMESEQ_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::KW_ELSE
		|| getPeek() == compiler::KW_FED || getPeek() == compiler::KW_FI
		|| getPeek() == compiler::KW_OD) {
		// EPISLON
		return STATEMENT() && STATEMESEQ_EXT();
	}

	//reportError("STATEMENTSEQ");
	return false;
}

bool TypeCheck::STATEMESEQ_EXT()
{
	if (getPeek() == compiler::SEMICOLON) {
		return matchToken(compiler::SEMICOLON) && STATEMESEQ();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::KW_ELSE
		|| getPeek() == compiler::KW_FED || getPeek() == compiler::KW_FI
		|| getPeek() == compiler::KW_OD) {
		// EPISLON
		return true;
	}

	//reportError("STATEMENTSEQ_EXT");
	return false;
}

bool TypeCheck::STATEMENT()
{
	if (getPeek() == compiler::ID) {
		return VAR() && matchToken(compiler::EQUAL)
			&& EXPR();
	}
	else if (getPeek() == compiler::KW_IF) {
		return matchToken(compiler::KW_IF) && BEXPR()
			&& matchToken(compiler::KW_THEN) && STATEMESEQ()
			&& STATEMENT_EXT();
	}
	else if (getPeek() == compiler::KW_PRINT) {
		return matchToken(compiler::KW_PRINT) && EXPR();
	}
	else if (getPeek() == compiler::KW_RETURN) {
		return matchToken(compiler::KW_RETURN) && EXPR();
	}
	else if (getPeek() == compiler::KW_WHILE) {
		return matchToken(compiler::KW_WHILE) && BEXPR()
			&& matchToken(compiler::KW_DO) && STATEMESEQ()
			&& matchToken(compiler::KW_OD);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		return true;
	}

	//reportError("STATEMENT");
	return false;
}

bool TypeCheck::STATEMENT_EXT()
{
	if (getPeek() == compiler::KW_ELSE) {
		return matchToken(compiler::KW_ELSE) && STATEMESEQ()
			&& matchToken(compiler::KW_FI);
	}
	else if (getPeek() == compiler::KW_FI) {
		return matchToken(compiler::KW_FI);
	}

	//reportError("STATEMENT_EXT");
	return false;
}

bool TypeCheck::EXPR()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {

		pushCategoryStack("expr");
		bool out = TERM() && EXPR_EXT();
		popCategoryStackTop();

		return out;
	}

	//reportError("EXPR");
	return false;
}

bool TypeCheck::EXPR_EXT()
{
	if (getPeek() == compiler::PLUS) {
		pushCategoryStack("expr_ext");
		bool out = matchToken(compiler::PLUS) && TERM()
			&& EXPR_EXT();
		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::MINUS) {
		pushCategoryStack("expr_ext");
		bool out = matchToken(compiler::MINUS) && TERM()
			&& EXPR_EXT();
		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::LEFT_BRACK
		|| getPeek() == compiler::COMMA || getPeek() == compiler::DOT
		|| getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::COMP_LTHAN || getPeek() == compiler::COMP_LEQUAL
		|| getPeek() == compiler::COMP_NOT || getPeek() == compiler::COMP_EQUAL
		|| getPeek() == compiler::COMP_GTHAN || getPeek() == compiler::COMP_GEQUAL
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		// EPSILON
		return true;
	}

	//reportError("EXPR_EXT");
	return false;
}

bool TypeCheck::TERM()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		return FACTOR() && TERM_EXT();
	}

	//reportError("TERM");
	return false;
}

bool TypeCheck::TERM_EXT()
{
	if (getPeek() == compiler::MODULUS) {
		pushCategoryStack("term_ext");
		bool out = matchToken(compiler::MODULUS) && FACTOR()
			&& TERM_EXT();
		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::MULTIPLY) {
		pushCategoryStack("term_ext");
		bool out = matchToken(compiler::MULTIPLY) && FACTOR()
			&& TERM_EXT();
		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::DIVIDE) {
		pushCategoryStack("term_ext");
		bool out = matchToken(compiler::DIVIDE) && FACTOR()
			&& TERM_EXT();
		popCategoryStackTop();
		return out;
	}
	else if (
		getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::LEFT_BRACK
		|| getPeek() == compiler::PLUS || getPeek() == compiler::MINUS
		|| getPeek() == compiler::COMMA || getPeek() == compiler::DOT
		|| getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::COMP_LTHAN || getPeek() == compiler::COMP_LEQUAL
		|| getPeek() == compiler::COMP_NOT || getPeek() == compiler::COMP_EQUAL
		|| getPeek() == compiler::COMP_GTHAN || getPeek() == compiler::COMP_GEQUAL
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		// EPSILON
		return true;
	}

	//reportError("TERM_EXT");
	return false;
}

bool TypeCheck::FACTOR()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchToken(compiler::LEFT_PAREN) && EXPR()
			&& matchToken(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::VALUE_INTEGER) {
		return NUMBER();
	}
	else if (getPeek() == compiler::ID) {
		pushCategoryStack("factor");
		bool out = matchToken(compiler::ID) && FACTOR_EXT();
		popCategoryStackTop();

		return out;
	}

	//reportError("FACTOR");
	return false;
}

bool TypeCheck::FACTOR_EXT()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchToken(compiler::LEFT_PAREN) && EXPRSEQ() && matchToken(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::MODULUS || getPeek() == compiler::RIGHT_PAREN
		|| getPeek() == compiler::LEFT_BRACK || getPeek() == compiler::MULTIPLY
		|| getPeek() == compiler::DIVIDE || getPeek() == compiler::PLUS
		|| getPeek() == compiler::MINUS || getPeek() == compiler::COMMA
		|| getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::COMP_LTHAN || getPeek() == compiler::COMP_LEQUAL
		|| getPeek() == compiler::COMP_NOT || getPeek() == compiler::COMP_EQUAL
		|| getPeek() == compiler::COMP_GTHAN || getPeek() == compiler::COMP_GEQUAL
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		return VAR_EXT();
	}

	//reportError("FACTOR_EXT");
	return false;
}

bool TypeCheck::EXPRSEQ()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		return EXPR() && EXPRSEQ_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		return true;
	}

	//reportError("EXPRSEQ");
	return false;
}

bool TypeCheck::EXPRSEQ_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchToken(compiler::COMMA) && EXPRSEQ();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		return true;
	}

	//reportError("EXPRSEQ_EXT");
	return false;
}

bool TypeCheck::BEXPR()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		pushCategoryStack("bexpr");
		bool out = BTERM() && BEXPR_EXT();
		popCategoryStackTop();
		return out;
	}

	//reportError("BEXPR");
	return false;
}

bool TypeCheck::BEXPR_EXT()
{
	if (getPeek() == compiler::KW_OR) {
		return matchToken(compiler::KW_OR) && BTERM()
			&& BEXPR_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::KW_DO
		|| getPeek() == compiler::KW_THEN) {
		// EPSILON
		return true;
	}

	//reportError("BEXPR_EXT");
	return false;
}

bool TypeCheck::BTERM()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		return BFACTOR() && BTERM_EXT();
	}

	//reportError("BTERM");
	return false;
}

bool TypeCheck::BTERM_EXT()
{
	if (getPeek() == compiler::KW_AND) {
		return matchToken(compiler::KW_AND) && BFACTOR()
			&& BTERM_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::KW_DO
		|| getPeek() == compiler::KW_OR || getPeek() == compiler::KW_THEN) {
		// EPSILON
		return true;
	}

	//reportError("BTERM_EXT");
	return false;
}

bool TypeCheck::BFACTOR()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchToken(compiler::LEFT_PAREN) && BFACTOR_EXT()
			&& matchToken(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::KW_NOT) {
		return matchToken(compiler::KW_NOT) && BFACTOR();
	}

	//reportError("BFACTOR");
	return false;
}

bool TypeCheck::BFACTOR_EXT()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		if (BEXPR()) {
			return true;
		}

		if (EXPR()) {
			return true;
		}
	}
	else if (getPeek() == compiler::KW_NOT) {
		return BEXPR();
	}
	else if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::ID
		|| getPeek() == compiler::VALUE_INTEGER) {
		return EXPR() && COMP()
			&& EXPR();
	}

	//reportError("BFACTOR_EXT");
	return false;
}

bool TypeCheck::COMP()
{
	if (getPeek() == compiler::COMP_LTHAN) {
		pushCategoryStack("comp");
		bool out = matchToken(compiler::COMP_LTHAN);
		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::COMP_LEQUAL) {
		pushCategoryStack("comp");
		bool out = matchToken(compiler::COMP_LEQUAL);
		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::COMP_NOT) {
		pushCategoryStack("comp");
		bool out = matchToken(compiler::COMP_NOT);
		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::COMP_EQUAL) {
		pushCategoryStack("comp");
		bool out = matchToken(compiler::COMP_EQUAL);
		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::COMP_GTHAN) {
		pushCategoryStack("comp");
		bool out = matchToken(compiler::COMP_GTHAN);
		popCategoryStackTop();
		return out;

	}
	else if (getPeek() == compiler::COMP_GEQUAL) {
		pushCategoryStack("comp");
		bool out = matchToken(compiler::COMP_GEQUAL);
		popCategoryStackTop();
		return out;
	}

	//reportError("COMP");
	return false;
}

bool TypeCheck::VAR()
{
	if (getPeek() == compiler::ID) {
		return matchToken(compiler::ID) && VAR_EXT();
	}

	//reportError("VAR");
	return false;
}

bool TypeCheck::VAR_EXT()
{
	if (getPeek() == compiler::LEFT_BRACK) {
		return matchToken(compiler::LEFT_BRACK) && EXPR()
			&& matchToken(compiler::LEFT_BRACK);
	}
	else if (getPeek() == compiler::MODULUS || getPeek() == compiler::RIGHT_PAREN || getPeek() == compiler::LEFT_BRACK
		|| getPeek() == compiler::MULTIPLY || getPeek() == compiler::DIVIDE
		|| getPeek() == compiler::PLUS || getPeek() == compiler::MINUS
		|| getPeek() == compiler::COMMA || getPeek() == compiler::DOT
		|| getPeek() == compiler::SEMICOLON || getPeek() == compiler::COMP_LTHAN
		|| getPeek() == compiler::COMP_LEQUAL || getPeek() == compiler::COMP_NOT
		|| getPeek() == compiler::EQUAL || getPeek() == compiler::COMP_EQUAL
		|| getPeek() == compiler::COMP_GTHAN || getPeek() == compiler::COMP_GEQUAL
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		// EPSILON
		return true;
	}

	//reportError("VAR_EXT");
	return false;
}

bool TypeCheck::NUMBER()
{
	if (getPeek() == compiler::VALUE_DOUBLE) {
		pushCategoryStack("number");
		bool out = matchToken(compiler::VALUE_DOUBLE);
		popCategoryStackTop();
		return out;
	}
	else if (getPeek() == compiler::VALUE_INTEGER) {
		pushCategoryStack("factor");
		bool out = matchToken(compiler::VALUE_INTEGER);
		popCategoryStackTop();
		return out;
	}

	//reportError("NUMBER");
	return false;
}
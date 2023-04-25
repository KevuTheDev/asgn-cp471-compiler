#include "Syntax.h"

Syntax::Syntax()
{
	this->_error = false;

	this->_tokenListIndex = 0;
	this->_tokenListLimit = 0;
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

compiler::TOKEN Syntax::getPeek()
{
	return this->_currentToken;
}

void Syntax::getNextToken()
{
	this->_tokenListIndex += 1;

	if (this->_tokenListIndex == this->_tokenListLimit) {
		this->_currentToken = compiler::DOLLAR_SIGN;
	}
	else {
		this->_currentToken = this->_tokenList->getToken(this->_tokenListIndex);
	}
}

bool Syntax::matchToken(compiler::TOKEN token)
{
	if (getPeek() != token) {
		return false;
	}

	getNextToken();
	return true;
}

void Syntax::reportError(const std::string& errorchar)
{
	uint32_t linenum = this->_tokenList->getLineNumber(this->_tokenListIndex);
	uint32_t charpos = this->_tokenList->getCharPosition(this->_tokenListIndex);
	compiler::TOKEN token = this->_tokenList->getToken(this->_tokenListIndex);
	std::string lexeme = this->_tokenList->getLexeme(this->_tokenListIndex);

	this->_logFileBuffer->logSyntaxError(linenum, charpos, lexeme);
}

void Syntax::start()
{
	bool hmm = PROGRAM();

	if (!hmm) {
		this->_error = true;
		std::cout << "INCOMPLETE" << std::endl;
		std::cout << "INCOMPLETE" << std::endl;
		std::cout << "INCOMPLETE" << std::endl;
		std::cout << "INCOMPLETE" << std::endl;
		std::cout << "INCOMPLETE" << std::endl;
		return;
	}

	std::cout << "COMPLETE" << std::endl;
	std::cout << "COMPLETE" << std::endl;
	std::cout << "COMPLETE" << std::endl;
	std::cout << "COMPLETE" << std::endl;
	std::cout << "COMPLETE" << std::endl;

}

bool Syntax::PROGRAM()
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

	reportError("PROGRAM");
	return false;
}

bool Syntax::FDECLS()
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

	reportError("FDECLS");
	return false;
}

bool Syntax::FDECLS_EXT()
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

	reportError("FDECLS_EXT");
	return false;
}

bool Syntax::FDEC()
{
	if (getPeek() == compiler::KW_DEF) {
		return matchToken(compiler::KW_DEF) && TYPE()
			&& FNAME() && matchToken(compiler::LEFT_PAREN)
			&& PARAMS() && matchToken(compiler::RIGHT_PAREN)
			&& DECLARATIONS() && STATEMESEQ()
			&& matchToken(compiler::KW_FED);
	}

	reportError("FDEC");
	return false;
}

bool Syntax::PARAMS()
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		return TYPE() && VAR()
			&& PARAMS_EXT();
	}

	reportError("PARAMS");
	return false;
}

bool Syntax::PARAMS_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchToken(compiler::COMMA) && PARAMS();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		return true;
	}

	reportError("PARAMS_EXT");
	return false;
}

bool Syntax::FNAME()
{
	if (getPeek() == compiler::ID) {
		return matchToken(compiler::ID);
	}

	reportError("FNAME");
	return false;
}

bool Syntax::DECLARATIONS()
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

	reportError("DECLARATIONS");
	return false;
}

bool Syntax::DECLARATIONS_EXT()
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

	reportError("DECLARATIONS_EXT");
	return false;
}

bool Syntax::DECL()
{
	if (getPeek() == compiler::KW_DOUBLE || getPeek() == compiler::KW_INT) {
		return TYPE() && VARLIST();
	}

	reportError("DECL");
	return false;
}

bool Syntax::TYPE()
{
	if (getPeek() == compiler::KW_DOUBLE) {
		return matchToken(compiler::KW_DOUBLE);
	}
	else if (getPeek() == compiler::KW_INT) {
		return matchToken(compiler::KW_INT);
	}

	reportError("TYPE");
	return false;
}

bool Syntax::VARLIST()
{
	if (getPeek() == compiler::ID) {
		return VAR() && VARLIST_EXT();
	}

	reportError("VARLIST");
	return false;
}

bool Syntax::VARLIST_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchToken(compiler::COMMA) && VARLIST();
	}
	else if (getPeek() == compiler::SEMICOLON) {
		// EPSILON
		return true;
	}

	reportError("VARLIST_EXT");
	return false;
}

bool Syntax::STATEMESEQ()
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

	reportError("STATEMENTSEQ");
	return false;
}

bool Syntax::STATEMESEQ_EXT()
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

	reportError("STATEMENTSEQ_EXT");
	return false;
}

bool Syntax::STATEMENT()
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

	reportError("STATEMENT");
	return false;
}

bool Syntax::STATEMENT_EXT()
{
	if (getPeek() == compiler::KW_ELSE) {
		return matchToken(compiler::KW_ELSE) && STATEMESEQ()
			&& matchToken(compiler::KW_FI);
	}
	else if (getPeek() == compiler::KW_FI) {
		return matchToken(compiler::KW_FI);
	}

	reportError("STATEMENT_EXT");
	return false;
}

bool Syntax::EXPR()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		return TERM() && EXPR_EXT();
	}

	reportError("EXPR");
	return false;
}

bool Syntax::EXPR_EXT()
{
	if (getPeek() == compiler::PLUS) {
		return matchToken(compiler::PLUS) && TERM()
			&& EXPR_EXT();
	}
	else if (getPeek() == compiler::MINUS) {
		return matchToken(compiler::MINUS) && TERM()
			&& EXPR_EXT();
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

	reportError("EXPR_EXT");
	return false;
}

bool Syntax::TERM()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		return FACTOR() && TERM_EXT();
	}

	reportError("TERM");
	return false;
}

bool Syntax::TERM_EXT()
{
	if (getPeek() == compiler::MODULUS) {
		return matchToken(compiler::MODULUS) && FACTOR()
			&& TERM_EXT();
	}
	else if (getPeek() == compiler::MULTIPLY) {
		return matchToken(compiler::MULTIPLY) && FACTOR()
			&& TERM_EXT();
	}
	else if (getPeek() == compiler::DIVIDE) {
		return matchToken(compiler::DIVIDE) && FACTOR()
			&& TERM_EXT();
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

	reportError("TERM_EXT");
	return false;
}

bool Syntax::FACTOR()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchToken(compiler::LEFT_PAREN) && EXPR()
			&& matchToken(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::VALUE_INTEGER) {
		return NUMBER();
	}
	else if (getPeek() == compiler::ID) {
		return matchToken(compiler::ID) && FACTOR_EXT();
	}

	reportError("FACTOR");
	return false;
}

bool Syntax::FACTOR_EXT()
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

	reportError("FACTOR_EXT");
	return false;
}

bool Syntax::EXPRSEQ()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		return EXPR() && EXPRSEQ_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		return true;
	}

	reportError("EXPRSEQ");
	return false;
}

bool Syntax::EXPRSEQ_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchToken(compiler::COMMA) && EXPRSEQ();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		return true;
	}

	reportError("EXPRSEQ_EXT");
	return false;
}

bool Syntax::BEXPR()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		return BTERM() && BEXPR_EXT();
	}

	reportError("BEXPR");
	return false;
}

bool Syntax::BEXPR_EXT()
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

	reportError("BEXPR_EXT");
	return false;
}

bool Syntax::BTERM()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		return BFACTOR() && BTERM_EXT();
	}

	reportError("BTERM");
	return false;
}

bool Syntax::BTERM_EXT()
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

	reportError("BTERM_EXT");
	return false;
}

bool Syntax::BFACTOR()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchToken(compiler::LEFT_PAREN) && BFACTOR_EXT()
			&& matchToken(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::KW_NOT) {
		return matchToken(compiler::KW_NOT) && BFACTOR();
	}

	reportError("BFACTOR");
	return false;
}

bool Syntax::BFACTOR_EXT()
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

	reportError("BFACTOR_EXT");
	return false;
}

bool Syntax::COMP()
{
	if (getPeek() == compiler::COMP_LTHAN) {
		return matchToken(compiler::COMP_LTHAN);
	}
	else if (getPeek() == compiler::COMP_LEQUAL) {
		return matchToken(compiler::COMP_LEQUAL);
	}
	else if (getPeek() == compiler::COMP_NOT) {
		return matchToken(compiler::COMP_NOT);
	}
	else if (getPeek() == compiler::COMP_EQUAL) {
		return matchToken(compiler::COMP_EQUAL);
	}
	else if (getPeek() == compiler::COMP_GTHAN) {
		return matchToken(compiler::COMP_GTHAN);

	}
	else if (getPeek() == compiler::COMP_GEQUAL) {
		return matchToken(compiler::COMP_GEQUAL);
	}

	reportError("COMP");
	return false;
}

bool Syntax::VAR()
{
	if (getPeek() == compiler::ID) {
		return matchToken(compiler::ID) && VAR_EXT();
	}

	reportError("VAR");
	return false;
}

bool Syntax::VAR_EXT()
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

	reportError("VAR_EXT");
	return false;
}

bool Syntax::NUMBER()
{
	if (getPeek() == compiler::VALUE_DOUBLE) {
		return matchToken(compiler::VALUE_DOUBLE);
	}
	else if (getPeek() == compiler::VALUE_INTEGER) {
		return matchToken(compiler::VALUE_INTEGER);
	}

	reportError("NUMBER");
	return false;
}
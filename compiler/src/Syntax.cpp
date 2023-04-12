#include "Syntax.h"

Syntax::Syntax()
{
	this->_position = 0;
	this->_limit = 0;
	this->_logFileBuffer = nullptr;
	this->_tokenFileBuffer = nullptr;
	this->_reservedWords = nullptr;
	this->_symbolTable = nullptr;
}

Syntax::~Syntax()
{
	this->_logFileBuffer = nullptr;
	this->_tokenFileBuffer = nullptr;
	this->_reservedWords = nullptr;
	this->_symbolTable = nullptr;
}

void Syntax::linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer)
{
	this->_logFileBuffer = buffer;
}

void Syntax::linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer)
{
	this->_tokenFileBuffer = buffer;
}

void Syntax::linkReservedWords(std::shared_ptr<ReservedWords> table)
{
	this->_reservedWords = table;
}

void Syntax::linkSymbolTable(std::shared_ptr<SymbolTable> table)
{
	this->_symbolTable = table;
	this->_limit = this->_symbolTable->length();
	std::cout << this->_limit << std::endl;
	this->_peek = this->_symbolTable->getTokenAtIndex(this->_position);;
}

void Syntax::run()
{
	this->start();
}

bool Syntax::getError()
{
	return this->_error;
}

void Syntax::matchToken(compiler::TOKEN token)
{
	if (token == this->_peek) {
		getNextToken();
	}
}
bool Syntax::matchTokenNew(compiler::TOKEN token)
{
	if (getPeek() != token) {
		return false;
	}

	getNextToken();
	return true;
}

void Syntax::getNextToken()
{
	readString += " " + this->_peek;
	this->_position += 1;

	if (this->_position == this->_limit) {
		this->_peek = compiler::DOLLAR_SIGN;
	}
	else {
		this->_peek = this->_symbolTable->getTokenAtIndex(this->_position);
	}
}

compiler::TOKEN Syntax::getPeek()
{
	return this->_peek;
}

void Syntax::printSyntaxError(std::string code)
{
	SymbolRow sr = this->_symbolTable->getSymbolRowAtIndex(this->_position);

	this->_logFileBuffer->logSyntaxError(sr.lineNumber, sr.charNumber, sr.lexeme);
}


void Syntax::start()
{
	bool hmm = PROGRAM();

	if (!hmm) {
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
		|| getPeek() == compiler::KW_DEF || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_INT
		|| getPeek() == compiler::KW_PRINT || getPeek() == compiler::KW_RETURN
		|| getPeek() == compiler::KW_WHILE) {
		return FDECLS() && DECLARATIONS()
			&& STATEMESEQ() && matchTokenNew(compiler::DOT);
	}

	printSyntaxError("PROGRAM");
	return false;
}

bool Syntax::FDECLS()
{
	if (getPeek() == compiler::KW_DEF) {
		return FDEC() && matchTokenNew(compiler::SEMICOLON)
			&& FDECLS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_DEF
		|| getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::KW_IF
		|| getPeek() == compiler::KW_INT || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return FDECLS_EXT();
	}

	printSyntaxError("FDECLS");
	return false;
}

bool Syntax::FDECLS_EXT()
{
	if (getPeek() == compiler::KW_DEF) {
		return FDEC() && matchTokenNew(compiler::SEMICOLON) && FDECLS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_INT
		|| getPeek() == compiler::KW_PRINT || getPeek() == compiler::KW_RETURN
		|| getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return true;
	}

	printSyntaxError("FDECLS_EXT");
	return false;
}

bool Syntax::FDEC()
{
	if (getPeek() == compiler::KW_DEF) {
		return matchTokenNew(compiler::KW_DEF) && TYPE()
			&& FNAME() && matchTokenNew(compiler::LEFT_PAREN)
			&& PARAMS() && matchTokenNew(compiler::RIGHT_PAREN)
			&& DECLARATIONS() && STATEMESEQ()
			&& matchTokenNew(compiler::KW_FED);
	}

	printSyntaxError("FDEC");
	return false;
}

bool Syntax::PARAMS()
{
	if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::KW_INT) {
		return TYPE() && VAR() 
			&& PARAMS_EXT();
	}

	printSyntaxError("PARAMS");
	return false;
}

bool Syntax::PARAMS_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchTokenNew(compiler::COMMA) && PARAMS();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		return true;
	}

	printSyntaxError("PARAMS_EXT");
	return false;
}

bool Syntax::FNAME()
{
	if (getPeek() == compiler::ID) {
		return matchTokenNew(compiler::ID);
	}

	printSyntaxError("FNAME");
	return false;
}

bool Syntax::DECLARATIONS()
{
	if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::KW_INT) {
		return DECL() && matchTokenNew(compiler::SEMICOLON) 
			&& DECLARATIONS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT 
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return DECLARATIONS_EXT();
	}

	printSyntaxError("DECLARATIONS");
	return false;
}

bool Syntax::DECLARATIONS_EXT()
{
	if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::KW_INT) {
		return DECL() && matchTokenNew(compiler::SEMICOLON)
			&& DECLARATIONS_EXT();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::ID || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_IF || getPeek() == compiler::KW_PRINT
		|| getPeek() == compiler::KW_RETURN || getPeek() == compiler::KW_WHILE) {
		// EPSILON
		return true;
	}

	printSyntaxError("DECLARATIONS_EXT");
	return false;
}

bool Syntax::DECL()
{
	if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::KW_INT) {
		return TYPE() && VARLIST();
	}

	printSyntaxError("DECL");
	return false;
}

bool Syntax::TYPE()
{
	if (getPeek() == compiler::VALUE_DOUBLE) {
		return matchTokenNew(compiler::VALUE_DOUBLE);
	}
	else if (getPeek() == compiler::KW_INT) {
		return matchTokenNew(compiler::KW_INT);
	}

	printSyntaxError("TYPE");
	return false;
}

bool Syntax::VARLIST()
{
	if (getPeek() == compiler::ID) {
		return VAR() && VARLIST_EXT();
	}

	printSyntaxError("VARLIST");
	return false;
}

bool Syntax::VARLIST_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchTokenNew(compiler::COMMA) && VARLIST();
	}
	else if (getPeek() == compiler::SEMICOLON) {
		// EPSILON
		return true;
	}

	printSyntaxError("VARLIST_EXT");
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

	printSyntaxError("STATEMENTSEQ");
	return false;
}

bool Syntax::STATEMESEQ_EXT()
{
	if (getPeek() == compiler::SEMICOLON) {
		return matchTokenNew(compiler::SEMICOLON) && STATEMESEQ();
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::KW_ELSE
		|| getPeek() == compiler::KW_FED || getPeek() == compiler::KW_FI
		|| getPeek() == compiler::KW_OD) {
		// EPISLON
		return true;
	}

	printSyntaxError("STATEMENTSEQ_EXT");
	return false;
}

bool Syntax::STATEMENT()
{
	if (getPeek() == compiler::ID) {
		return VAR() && matchTokenNew(compiler::EQUAL)
			&& EXPR();
	}
	else if (getPeek() == compiler::KW_IF) {
		return matchTokenNew(compiler::KW_IF) && BEXPR()
			&& matchTokenNew(compiler::KW_THEN) && STATEMESEQ()
			&& STATEMENT_EXT();
	}
	else if (getPeek() == compiler::KW_PRINT) {
		return matchTokenNew(compiler::KW_PRINT) && EXPR();
	}
	else if (getPeek() == compiler::KW_RETURN) {
		return matchTokenNew(compiler::KW_RETURN) && EXPR();
	}
	else if (getPeek() == compiler::KW_WHILE) {
		return matchTokenNew(compiler::KW_WHILE) && BEXPR()
			&& matchTokenNew(compiler::KW_DO) && STATEMESEQ()
			&& matchTokenNew(compiler::KW_OD);
	}
	else if (getPeek() == compiler::DOT || getPeek() == compiler::SEMICOLON
		|| getPeek() == compiler::KW_ELSE || getPeek() == compiler::KW_FED
		|| getPeek() == compiler::KW_FI || getPeek() == compiler::KW_OD) {
		return true;
	}

	printSyntaxError("STATEMENT");
	return false;
}

bool Syntax::STATEMENT_EXT()
{
	if (getPeek() == compiler::KW_ELSE) {
		return matchTokenNew(compiler::KW_ELSE) && STATEMESEQ()
			&& matchTokenNew(compiler::KW_FI);
	}
	else if (getPeek() == compiler::KW_FI) {
		return matchTokenNew(compiler::KW_FI);
	}

	printSyntaxError("STATEMENT_EXT");
	return false;
}

bool Syntax::EXPR()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		return TERM() && EXPR_EXT();
	}

	printSyntaxError("EXPR");
	return false;
}

bool Syntax::EXPR_EXT()
{
	if (getPeek() == compiler::PLUS) {
		return matchTokenNew(compiler::PLUS) && TERM()
			&& EXPR_EXT();
	}
	else if (getPeek() == compiler::MINUS) {
		return matchTokenNew(compiler::MINUS) && TERM()
			&& EXPR_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN|| getPeek() == compiler::LEFT_BRACK
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

	printSyntaxError("EXPR_EXT");
	return false;
}

bool Syntax::TERM()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::VALUE_DOUBLE 
		|| getPeek() == compiler::ID || getPeek() == compiler::VALUE_INTEGER) {
		return FACTOR() && TERM_EXT();
	}

	printSyntaxError("TERM");
	return false;
}

bool Syntax::TERM_EXT()
{
	if (getPeek() == compiler::MODULUS) {
		return matchTokenNew(compiler::MODULUS) && FACTOR()
			&& TERM_EXT();
	}
	else if (getPeek() == compiler::MULTIPLY) {
		return matchTokenNew(compiler::MULTIPLY) && FACTOR()
			&& TERM_EXT();
	}
	else if (getPeek() == compiler::DIVIDE) {
		return matchTokenNew(compiler::DIVIDE) && FACTOR()
			&& TERM_EXT();
	}
	else if (
		getPeek() == compiler::RIGHT_PAREN|| getPeek() == compiler::LEFT_BRACK
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

	printSyntaxError("TERM_EXT");
	return false;
}

bool Syntax::FACTOR()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchTokenNew(compiler::LEFT_PAREN) && EXPR() 
			&& matchTokenNew(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::VALUE_DOUBLE || getPeek() == compiler::VALUE_INTEGER) {
		return NUMBER();
	}
	else if (getPeek() == compiler::ID) {
		return matchTokenNew(compiler::ID) && FACTOR_EXT();
	}

	printSyntaxError("FACTOR");
	return false;
}

bool Syntax::FACTOR_EXT()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchTokenNew(compiler::LEFT_PAREN) && EXPRSEQ() && matchTokenNew(compiler::RIGHT_PAREN);
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

	printSyntaxError("FACTOR_EXT");
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

	printSyntaxError("EXPRSEQ");
	return false;
}

bool Syntax::EXPRSEQ_EXT()
{
	if (getPeek() == compiler::COMMA) {
		return matchTokenNew(compiler::COMMA) && EXPRSEQ();
	}
	else if (getPeek() == compiler::RIGHT_PAREN) {
		// EPSILON
		return true;
	}

	printSyntaxError("EXPRSEQ_EXT");
	return false;
}

bool Syntax::BEXPR()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		return BTERM() && BEXPR_EXT();
	}

	printSyntaxError("BEXPR");
	return false;
}

bool Syntax::BEXPR_EXT()
{
	if (getPeek() == compiler::KW_OR) {
		return matchTokenNew(compiler::KW_OR) && BTERM()
			&& BEXPR_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN|| getPeek() == compiler::KW_DO
		|| getPeek() == compiler::KW_THEN) {
		// EPSILON
		return true;
	}

	printSyntaxError("BEXPR_EXT");
	return false;
}

bool Syntax::BTERM()
{
	if (getPeek() == compiler::LEFT_PAREN || getPeek() == compiler::KW_NOT) {
		return BFACTOR() && BTERM_EXT();
	}

	printSyntaxError("BTERM");
	return false;
}

bool Syntax::BTERM_EXT()
{
	if (getPeek() == compiler::KW_AND) {
		return matchTokenNew(compiler::KW_AND) && BFACTOR()
			&& BTERM_EXT();
	}
	else if (getPeek() == compiler::RIGHT_PAREN|| getPeek() == compiler::KW_DO
		|| getPeek() == compiler::KW_OR || getPeek() == compiler::KW_THEN) {
		// EPSILON
		return true;
	}

	printSyntaxError("BTERM_EXT");
	return false;
}

bool Syntax::BFACTOR()
{
	if (getPeek() == compiler::LEFT_PAREN) {
		return matchTokenNew(compiler::LEFT_PAREN) && BFACTOR_EXT()
			&& matchTokenNew(compiler::RIGHT_PAREN);
	}
	else if (getPeek() == compiler::KW_NOT) {
		return matchTokenNew(compiler::KW_NOT) && BFACTOR();
	}

	printSyntaxError("BFACTOR");
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

	printSyntaxError("BFACTOR_EXT");
	return false;
}

bool Syntax::COMP()
{
	if (getPeek() == compiler::COMP_LTHAN) {
		return matchTokenNew(compiler::COMP_LTHAN);
	}
	else if (getPeek() == compiler::COMP_LEQUAL) {
		return matchTokenNew(compiler::COMP_LEQUAL);
	}
	else if (getPeek() == compiler::COMP_NOT) {
		return matchTokenNew(compiler::COMP_NOT);
	}
	else if (getPeek() == compiler::COMP_EQUAL) {
		return matchTokenNew(compiler::COMP_EQUAL);
	}
	else if (getPeek() == compiler::COMP_GTHAN) {
		return matchTokenNew(compiler::COMP_GTHAN);

	}else if (getPeek() == compiler::COMP_GEQUAL) {
		return matchTokenNew(compiler::COMP_GEQUAL);
	}

	printSyntaxError("COMP");
	return false;
}

bool Syntax::VAR()
{
	if (getPeek() == compiler::ID) {
		return matchTokenNew(compiler::ID) && VAR_EXT();
	}

	printSyntaxError("VAR");
	return false;
}

bool Syntax::VAR_EXT()
{
	if (getPeek() == compiler::LEFT_BRACK) {
		return matchTokenNew(compiler::LEFT_BRACK) && EXPR()
			&& matchTokenNew(compiler::LEFT_BRACK);
	}
	else if (getPeek() == compiler::MODULUS || getPeek() == compiler::RIGHT_PAREN|| getPeek() == compiler::LEFT_BRACK
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

	printSyntaxError("VAR_EXT");
	return false;
}

bool Syntax::NUMBER()
{
	if (getPeek() == compiler::VALUE_DOUBLE) {
		return matchTokenNew(compiler::VALUE_DOUBLE);
	}
	else if (getPeek() == compiler::VALUE_INTEGER) {
		return matchTokenNew(compiler::VALUE_INTEGER);
	}

	printSyntaxError("NUMBER");
	return false;
}

bool Syntax::E()
{
	if (getPeek() == compiler::ID || getPeek() == compiler::LEFT_PAREN) {
		return T() && E1();
	}

	std::cout << "ERROR E" << std::endl;
	return false;
}

bool Syntax::E1()
{
	if (getPeek() == compiler::PLUS) {
		return matchTokenNew(compiler::PLUS) && T()
			&& E1();
	}
	else if (getPeek() == compiler::RIGHT_PAREN|| getPeek() == compiler::DOLLAR_SIGN) {
		// EPSILON VALUES = [compiler::RIGHT_PAREN, compiler::DOLLAR_SIGN]
		return true;
	}

	std::cout << "ERROR E1" << std::endl;
	return false;
}

bool Syntax::T()
{
	if (getPeek() == compiler::ID) {
		return F() && T1();
	}
	else if (getPeek() == compiler::LEFT_PAREN) {
		return F() && T1();
	}

	std::cout << "ERROR T" << std::endl;
	return false;
}

bool Syntax::T1()
{
	if (getPeek() == compiler::MULTIPLY) {
		return matchTokenNew(compiler::MULTIPLY) && F()
			&& T1();
	}
	else if (getPeek() == compiler::PLUS || getPeek() == compiler::RIGHT_PAREN
		|| getPeek() == compiler::DOLLAR_SIGN) {
		// EPSILON VALUES = [compiler::PLUS, compiler::RIGHT_PAREN, compiler::DOLLAR_SIGN]
		return true;
	}

	std::cout << "ERROR T1" << std::endl;
	return false;
}

bool Syntax::F()
{
	if (getPeek() == compiler::ID) {
		return matchTokenNew(compiler::ID);
	}
	else if (getPeek() == compiler::LEFT_PAREN) {
		return matchTokenNew(compiler::LEFT_PAREN) && E()
			&& matchTokenNew(compiler::RIGHT_PAREN);
	}

	std::cout << "ERROR F" << std::endl;
	return false;
}

/* Syntax Simple Grammar v2
bool Syntax::E()
{
	if (getPeek() != compiler::ID && getPeek() != compiler::LEFT_PAREN) {
		std::cout << "ERROR E" << std::endl;
		return false;
	}

	if (getPeek() == compiler::ID || getPeek() == compiler::LEFT_PAREN) {
		return T() && E1();
	}

	return true;
}

bool Syntax::E1()
{
	if (getPeek() != compiler::PLUS && getPeek() != compiler::RIGHT_PAREN
		&& getPeek() != compiler::DOLLAR_SIGN) {
		std::cout << "ERROR E1" << std::endl;
		return false;
	}

	if (getPeek() == compiler::PLUS) {
		return matchTokenNew(compiler::PLUS) && T()
			&& E1();
	}

	// EPSILON VALUES = [compiler::RIGHT_PAREN, compiler::DOLLAR_SIGN]
	return true;
}

bool Syntax::T()
{
	if (getPeek() != compiler::ID && getPeek() != compiler::LEFT_PAREN) {
		std::cout << "ERROR T" << std::endl;
		return false;
	}

	if (getPeek() == compiler::ID) {
		return F() && T1();
	}
	else if (getPeek() == compiler::LEFT_PAREN) {
		return F() && T1();
	}

	return true;
}

bool Syntax::T1()
{
	if (getPeek() != compiler::MULTIPLY && getPeek() != compiler::PLUS
		&& getPeek() != compiler::RIGHT_PAREN&& getPeek() != compiler::DOLLAR_SIGN) {
		std::cout << "ERROR T1" << std::endl;
		return false;
	}

	if (getPeek() == compiler::MULTIPLY) {
		return matchTokenNew(compiler::MULTIPLY) && F()
			&& T1();
	}

	// EPSILON VALUES = [compiler::PLUS, compiler::RIGHT_PAREN, compiler::DOLLAR_SIGN]
	return true;
}

bool Syntax::F()
{
	if (getPeek() != compiler::ID && getPeek() != compiler::LEFT_PAREN) {
		std::cout << "ERROR F" << std::endl;
		return false;
	}

	if (getPeek() == compiler::ID) {
		return matchTokenNew(compiler::ID);
	}
	else if (getPeek() == compiler::LEFT_PAREN) {
		return matchTokenNew(compiler::LEFT_PAREN) && E()
			&& matchTokenNew(compiler::RIGHT_PAREN);
	}

	return true;
}
*/

/* Syntax Simple Grammar v1
void Syntax::E()
{
	if (this->_peek == compiler::ID) {
		T();
		E1();
	}
	else if (this->_peek == compiler::LEFT_PAREN) {
		T();
		E1();
	}
	else {
		std::cout << "ERROR E" << std::endl;
	}
}

void Syntax::E1()
{
	if (this->_peek == compiler::PLUS) {
		matchToken(compiler::PLUS);
		T();
		E1();
	}
	else if (this->_peek == compiler::RIGHT_PAREN|| this->_peek == compiler::DOLLAR_SIGN) {

	}
	else {
		std::cout << "ERROR E1" << std::endl;
	}
}

void Syntax::T()
{
	if (this->_peek == compiler::ID) {
		F();
		T1();
	}
	else if (this->_peek == compiler::LEFT_PAREN) {
		F();
		T1();
	}
	else {
		std::cout << "ERROR T" << std::endl;
	}
}

void Syntax::T1()
{
	if (this->_peek == compiler::MULTIPLY) {
		matchToken(compiler::MULTIPLY);
		F();
		T1();
	}
	else if (this->_peek == compiler::PLUS || this->_peek == compiler::RIGHT_PAREN|| this->_peek == compiler::DOLLAR_SIGN) {

	}
	else {
		std::cout << "ERROR T1" << std::endl;
	}
}

void Syntax::F()
{
	if (this->_peek == compiler::ID) {
		matchToken(compiler::ID);
	}
	else if (this->_peek == compiler::LEFT_PAREN) {
		matchToken(compiler::LEFT_PAREN);
		E();
		matchToken(compiler::RIGHT_PAREN);
	}
	else {
		std::cout << "ERROR F" << std::endl;
	}
}
*/
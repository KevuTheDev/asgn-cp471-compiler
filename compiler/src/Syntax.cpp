#include "Syntax.h"

extern std::unique_ptr<ReservedWords> RESERVED_WORDS;
extern std::unique_ptr<SymbolTable> SYMBOL_TABLE;
extern std::unique_ptr<LogFileBuffer> LOG_FILE_BUFFER;
extern std::unique_ptr<TokenFileBuffer> TOKEN_FILE_BUFFER;

Syntax::Syntax()
{
	this->_position = 0;
	this->_limit = ::SYMBOL_TABLE->length();

	std::cout << this->_limit << std::endl;

	this->_peek = ::SYMBOL_TABLE->getTokenAtIndex(this->_position);;
}

void Syntax::start()
{
	bool hmm = E();

	if (hmm) {
		std::cout << "COMPLETE" << std::endl;
		std::cout << "COMPLETE" << std::endl;
		std::cout << "COMPLETE" << std::endl;
		std::cout << "COMPLETE" << std::endl;
		std::cout << "COMPLETE" << std::endl;
	}
	else {
		std::cout << "INCOMPLETE" << std::endl;
		std::cout << "INCOMPLETE" << std::endl;
		std::cout << "INCOMPLETE" << std::endl;
		std::cout << "INCOMPLETE" << std::endl;
		std::cout << "INCOMPLETE" << std::endl;
	}
}

void Syntax::matchToken(const std::string& token)
{
	if (token == this->_peek) {
		getNextToken();
	}
}
bool Syntax::matchTokenNew(const std::string& token)
{
	if (getPeek() != token) {
		return false;
	}

	getNextToken();
	return true;
}

void Syntax::getNextToken()
{
	this->_position += 1;

	if (this->_position == this->_limit) {
		this->_peek = "$";
	}
	else {
		this->_peek = ::SYMBOL_TABLE->getTokenAtIndex(this->_position);
	}
}

std::string Syntax::getPeek()
{
	return this->_peek;
}

bool Syntax::NT_PROGRAM()
{
	if (getPeek() == "ID" || getPeek() == "def"
		|| getPeek() == "double" || getPeek() == "if"
		|| getPeek() == "int" || getPeek() == "print"
		|| getPeek() == "return" || getPeek() == "while"
		|| getPeek() == "$") {
		NT_FDECLS();
		NT_DECLARATIONS();
		NT_STATEMENTSEQ();
		matchToken(".");
		return true;
	}
	return false;
}

bool Syntax::NT_FDECLS()
{
	if (getPeek() == "def") {
		NT_FDEC();
		matchToken(";");
		NT_FDECLS();
		return true;
	}
	else if (getPeek() == "ID" || getPeek() == "double"
		|| getPeek() == "if" || getPeek() == "int" 
		|| getPeek() == "print" || getPeek() == "return" 
		|| getPeek() == "while") {

		// EPSILON
	}
	return false;
}

bool Syntax::NT_FDEC()
{
	if (getPeek() == "def") {
		matchToken("def");
		NT_TYPE();
		NT_FNAME();
		matchToken("(");
		NT_PARAMS();
		matchToken(")");
		NT_DECLARATIONS();
		NT_STATEMENTSEQ();
		matchToken("fed");
		return true;
	}
	return false;
}

bool Syntax::NT_PARAMS()
{
	if (getPeek() == "double" || getPeek() == "int") {
		NT_TYPE();
		NT_VAR();
		NT_PARAMS_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_PARAMS_EXT()
{
	if (getPeek() == ",") {
		matchToken(",");
		NT_PARAMS();
		return true;
	}
	else if (getPeek() == ")") {
		// EPSILON
	}
	return false;
}

bool Syntax::NT_FNAME()
{
	if (getPeek() == "ID") {
		matchToken("ID");
		return true;
	}
	return false;
}

bool Syntax::NT_DECLARATIONS()
{
	if (getPeek() == "double" || getPeek() == "int") {
		NT_DECL();
		matchToken(";");
		NT_DECLARATIONS();
		return true;
	}
	else if (getPeek() == "ID" || getPeek() == "id" 
		|| getPeek() == "print" || getPeek() == "return"
		|| getPeek() == "while") {
		// EPSILON

	}
	return false;
}

bool Syntax::NT_DECL()
{
	if (getPeek() == "double" || getPeek() == "int") {
		NT_TYPE();
		NT_VARLIST();
		return true;
	}
	return false;
}

bool Syntax::NT_TYPE()
{
	if (getPeek() == "double") {
		matchToken("double");
		return true;
	}
	else if (getPeek() == "int") {
		matchToken("int");
		return true;
	}
	return false;
}

bool Syntax::NT_VARLIST()
{
	if (getPeek() == "ID") {
		NT_VAR();
		NT_VARLIST_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_VARLIST_EXT()
{
	if (getPeek() == ",") {
		matchToken(",");
		NT_VARLIST();
		return true;
	}
	else if (getPeek() == ";") {
		// EPSILON
	}
	return false;
}

bool Syntax::NT_STATEMENT()
{
	if (getPeek() == "ID") {
		NT_VAR();
		matchToken("=");
		NT_EXPR();
		return true;
	}
	else if (getPeek() == "if") {
		matchToken("if");
		NT_BEXPR();
		matchToken("then");
		NT_STATEMENTSEQ();
		NT_STATEMENT_EXT();
		return true;
	}
	else if (getPeek() == "print") {
		matchToken("print");
		NT_EXPR();
		return true;
	}
	else if (getPeek() == "return") {
		matchToken("return");
		NT_EXPR();
		return true;
	}
	else if (getPeek() == "while") {
		matchToken("while");
		NT_BEXPR();
		matchToken("do");
		NT_STATEMENTSEQ();
		matchToken("od");
		return true;
	}
	return false;
}

bool Syntax::NT_STATEMENT_EXT()
{
	if (getPeek() == "else") {
		matchToken("else");
		NT_STATEMENTSEQ();
		matchToken("fi");
		return true;
	}
	else if (getPeek() == "fi") {
		matchToken("fi");
		return true;
	}
	return false;
}

bool Syntax::NT_STATEMENTSEQ()
{
	if (getPeek() == "ID" || getPeek() == "if"
		|| getPeek() == "print" || getPeek() == "return"
		|| getPeek() == "while") {
		NT_STATEMENT();
		NT_STATEMENTSEQ_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_STATEMENTSEQ_EXT()
{
	if (getPeek() == ";") {
		matchToken(";");
		NT_STATEMENTSEQ();
		return true;
	}
	else if (getPeek() == "." || getPeek() == "else"
		|| getPeek() == "fed" || getPeek() == "fi"
		|| getPeek() == "fi" || getPeek() == "od") {
		// EPISLON
	}
	return false;
}

bool Syntax::NT_EXPR()
{
	if (getPeek() == "(" || getPeek() == "DOUBLE"
		|| getPeek() == "ID" || getPeek() == "INTEGER") {
		NT_TERM();
		NT_EXPR_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_EXPR_EXT()
{
	if (getPeek() == "+") {
		matchToken("+");
		NT_TERM();
		NT_EXPR_EXT();
		return true;
	}
	else if (getPeek() == "-") {
		matchToken("-");
		NT_TERM();
		NT_EXPR_EXT();
		return true;
	}
	else if (getPeek() == ")" || getPeek() == "]"
		|| getPeek() == "," || getPeek() == "."
		|| getPeek() == "<" || getPeek() == "<="
		|| getPeek() == "<>" || getPeek() == "=="
		|| getPeek() == ">" || getPeek() == ">="
		|| getPeek() == "else" || getPeek() == "fed"
		|| getPeek() == "fi" || getPeek() == "od") {
		// EPSILON
	}
	return false;
}

bool Syntax::NT_TERM()
{
	if (getPeek() == "(" || getPeek() == "DOUBLE" 
		|| getPeek() == "ID" || getPeek() == "INTEGER") {
		NT_FACTOR();
		NT_TERM_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_TERM_EXT()
{
	if (getPeek() == "%") {
		matchToken("%");
		NT_FACTOR();
		NT_TERM_EXT();
		return true;
	}
	else if (getPeek() == "*") {
		matchToken("*");
		NT_FACTOR();
		NT_TERM_EXT();
		return true;
	}
	else if (getPeek() == "/") {
		matchToken("/");
		NT_FACTOR();
		NT_TERM_EXT();
		return true;
	}
	else if (getPeek() == ")" || getPeek() == "+"
		|| getPeek() == "-" || getPeek() == "."
		|| getPeek() == ";" || getPeek() == "<"
		|| getPeek() == "<=" || getPeek() == "<>"
		|| getPeek() == "==" || getPeek() == ">"
		|| getPeek() == ">=" || getPeek() == "else"
		|| getPeek() == "fed" || getPeek() == "fi"
		|| getPeek() == "od") {
		// EPSILON
	}
	return false;
}

bool Syntax::NT_FACTOR()
{
	if (getPeek() == "(") {
		matchToken("(");
		NT_EXPR();
		matchToken(")");
		return true;
	}
	else if (getPeek() == "DOUBLE" || getPeek() == "INTEGER") {
		NT_NUMBER();
		return true;
	}
	else if (getPeek() == "ID") {
		matchToken("ID");
		NT_FACTOR_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_FACTOR_EXT()
{
	if (getPeek() == "(") {
		matchToken("(");
		NT_EXPRSEQ();
		return true;
	}
	else if (getPeek() == "%" || getPeek() == ")"
		|| getPeek() == "[" || getPeek() == "*"
		|| getPeek() == "/" || getPeek() == "+"
		|| getPeek() == "-" || getPeek() == ","
		|| getPeek() == "." || getPeek() == ";"
		|| getPeek() == "<" || getPeek() == "<="
		|| getPeek() == "<>" || getPeek() == "=="
		|| getPeek() == ">" || getPeek() == ">="
		|| getPeek() == "else" || getPeek() == "fed"
		|| getPeek() == "fi" || getPeek() == "od") {
		NT_VAR_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_EXPRSEQ()
{
	if (getPeek() == "(" || getPeek() == "DOUBLE"
		|| getPeek() == "ID" || getPeek() == "INTEGER") {
		NT_EXPR();
		NT_EXPRSEQ_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_EXPRSEQ_EXT()
{
	if (getPeek() == ",") {
		matchToken(",");
		NT_EXPRSEQ();
		return true;
	}
	else if (getPeek() == ")") {
		// EPSILON
	}
	return false;
}

bool Syntax::NT_BEXPR()
{
	if (getPeek() == "(" || getPeek() == "not") {
		NT_BTERM();
		NT_BEXPR_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_BEXPR_EXT()
{
	if (getPeek() == "or") {
		matchToken("or");
		NT_BTERM();
		NT_BEXPR_EXT();
		return true;
	}
	else if (getPeek() == ")" || getPeek() == "do"
		|| getPeek() == "then") {
		// EPSILON
	}
	return false;
}

bool Syntax::NT_BTERM()
{
	if (getPeek() == "(" || getPeek() == "not") {
		NT_BFACTOR();
		NT_BTERM_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_BTERM_EXT()
{
	if (getPeek() == "and") {
		matchToken("and");
		NT_BFACTOR();
		NT_BTERM_EXT();
		return true;
	}
	else if (getPeek() == ")" || getPeek() == "do"
		|| getPeek() == "or" || getPeek() == "then") {
		// EPSILON
	}
	return false;
}

bool Syntax::NT_BFACTOR()
{
	if (getPeek() == "(") {
		matchToken("(");
		NT_BFACTOR_EXT();
		matchToken(")");
		return true;
	}
	else if (getPeek() == "not") {
		matchToken("not");
		NT_BFACTOR();
		return true;
	}
	return false;
}

bool Syntax::NT_BFACTOR_EXT()
{
	if (NT_BEXPR()) {
		return true;
	}
	if (NT_EXPR()) {
		return true;
	}
	if (getPeek() == "DOUBLE" || getPeek() == "ID"
		|| getPeek() == "INTEGER") {
		NT_EXPR();
		NT_COMP();
		NT_EXPR();
		return true;
	}
	return false;
}

bool Syntax::NT_COMP()
{
	if (getPeek() == "<") {
		matchToken("<");
		return true;
	}
	else if (getPeek() == "<=") {
		matchToken("<=");
		return true;
	}
	else if (getPeek() == "<>") {
		matchToken("<>");
		return true;
	}
	else if (getPeek() == "==") {
		matchToken("==");
		return true;
	}
	else if (getPeek() == ">") {
		matchToken(">");
		return true;

	}else if (getPeek() == ">=") {
		matchToken(">=");
		return true;
	}
	return false;
}

bool Syntax::NT_VAR()
{
	if (getPeek() == "ID") {
		matchToken("ID");
		NT_VAR_EXT();
		return true;
	}
	return false;
}

bool Syntax::NT_VAR_EXT()
{
	if (getPeek() == "[") {
		matchToken("[");
		NT_EXPR();
		matchToken("]");
		return true;
	}
	else if (getPeek() == "%" || getPeek() == ")"
		|| getPeek() == "*" || getPeek() == "/"
		|| getPeek() == "+" || getPeek() == "-"
		|| getPeek() == "," || getPeek() == "."
		|| getPeek() == ";" || getPeek() == "<"
		|| getPeek() == "<=" || getPeek() == "<>"
		|| getPeek() == "=" || getPeek() == "=="
		|| getPeek() == ">" || getPeek() == ">="
		|| getPeek() == "else" || getPeek() == "fed"
		|| getPeek() == "fi" || getPeek() == "od") {
		// EPSILON
	}

	return false;
}

bool Syntax::NT_NUMBER()
{
	if (getPeek() != "DOUBLE" && getPeek() != "INTEGER") {
		std::cout << "ERROR::NUMBER" << std::endl;
		return false;
	}

	if (getPeek() == "DOUBLE") {
		return matchTokenNew("DOUBLE");
	}
	else if (getPeek() == "INTEGER") {
		return matchTokenNew("INTEGER");
	}

	return true;
}

bool Syntax::E()
{
	if (getPeek() == "ID" || getPeek() == "(") {
		return T() && E1();
	}

	std::cout << "ERROR E" << std::endl;
	return false;
}

bool Syntax::E1()
{
	if (getPeek() == "+") {
		return matchTokenNew("+") && T()
			&& E1();
	}
	else if (getPeek() == ")" || getPeek() == "$") {
		// EPSILON VALUES = [")", "$"]
		return true;
	}

	std::cout << "ERROR E1" << std::endl;
	return false;
}

bool Syntax::T()
{
	if (getPeek() == "ID") {
		return F() && T1();
	}
	else if (getPeek() == "(") {
		return F() && T1();
	}

	std::cout << "ERROR T" << std::endl;
	return false;
}

bool Syntax::T1()
{
	if (getPeek() == "*") {
		return matchTokenNew("*") && F()
			&& T1();
	}
	else if (getPeek() == "+" || getPeek() == ")" 
		|| getPeek() == "$") {
		// EPSILON VALUES = ["+", ")", "$"]
		return true;
	}

	std::cout << "ERROR T1" << std::endl;
	return false;
}

bool Syntax::F()
{
	if (getPeek() == "ID") {
		return matchTokenNew("ID");
	}
	else if (getPeek() == "(") {
		return matchTokenNew("(") && E()
			&& matchTokenNew(")");
	}

	std::cout << "ERROR F" << std::endl;
	return false;
}

/* Syntax Simple Grammar v2
bool Syntax::E()
{
	if (getPeek() != "ID" && getPeek() != "(") {
		std::cout << "ERROR E" << std::endl;
		return false;
	}

	if (getPeek() == "ID" || getPeek() == "(") {
		return T() && E1();
	}

	return true;
}

bool Syntax::E1()
{
	if (getPeek() != "+" && getPeek() != ")"
		&& getPeek() != "$") {
		std::cout << "ERROR E1" << std::endl;
		return false;
	}

	if (getPeek() == "+") {
		return matchTokenNew("+") && T()
			&& E1();
	}

	// EPSILON VALUES = [")", "$"]
	return true;
}

bool Syntax::T()
{
	if (getPeek() != "ID" && getPeek() != "(") {
		std::cout << "ERROR T" << std::endl;
		return false;
	}

	if (getPeek() == "ID") {
		return F() && T1();
	}
	else if (getPeek() == "(") {
		return F() && T1();
	}

	return true;
}

bool Syntax::T1()
{
	if (getPeek() != "*" && getPeek() != "+"
		&& getPeek() != ")" && getPeek() != "$") {
		std::cout << "ERROR T1" << std::endl;
		return false;
	}

	if (getPeek() == "*") {
		return matchTokenNew("*") && F()
			&& T1();
	}

	// EPSILON VALUES = ["+", ")", "$"]
	return true;
}

bool Syntax::F()
{
	if (getPeek() != "ID" && getPeek() != "(") {
		std::cout << "ERROR F" << std::endl;
		return false;
	}

	if (getPeek() == "ID") {
		return matchTokenNew("ID");
	}
	else if (getPeek() == "(") {
		return matchTokenNew("(") && E()
			&& matchTokenNew(")");
	}

	return true;
}
*/

/* Syntax Simple Grammar v1
void Syntax::E()
{
	if (this->_peek == "id") {
		T();
		E1();
	}
	else if (this->_peek == "(") {
		T();
		E1();
	}
	else {
		std::cout << "ERROR E" << std::endl;
	}
}

void Syntax::E1()
{
	if (this->_peek == "+") {
		matchToken("+");
		T();
		E1();
	}
	else if (this->_peek == ")" || this->_peek == "$") {

	}
	else {
		std::cout << "ERROR E1" << std::endl;
	}
}

void Syntax::T()
{
	if (this->_peek == "id") {
		F();
		T1();
	}
	else if (this->_peek == "(") {
		F();
		T1();
	}
	else {
		std::cout << "ERROR T" << std::endl;
	}
}

void Syntax::T1()
{
	if (this->_peek == "*") {
		matchToken("*");
		F();
		T1();
	}
	else if (this->_peek == "+" || this->_peek == ")" || this->_peek == "$") {

	}
	else {
		std::cout << "ERROR T1" << std::endl;
	}
}

void Syntax::F()
{
	if (this->_peek == "id") {
		matchToken("id");
	}
	else if (this->_peek == "(") {
		matchToken("(");
		E();
		matchToken(")");
	}
	else {
		std::cout << "ERROR F" << std::endl;
	}
}
*/
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
	bool hmm = NT_PROGRAM();

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
	readString += " " + this->_peek;
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
		return NT_FDECLS() && NT_DECLARATIONS()
			&& NT_STATEMENTSEQ() && matchTokenNew(".");
	}

	std::cout << "ERROR::PROGRAM" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_FDECLS()
{
	if (getPeek() == "def") {
		return NT_FDEC() && matchTokenNew(";")
			&& NT_FDECLS();
	}
	else if (getPeek() == "ID" || getPeek() == "double"
		|| getPeek() == "if" || getPeek() == "int" 
		|| getPeek() == "print" || getPeek() == "return" 
		|| getPeek() == "while") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::FDECLS" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_FDEC()
{
	if (getPeek() == "def") {
		return matchTokenNew("def") && NT_TYPE()
			&& NT_FNAME() && matchTokenNew("(")
			&& NT_PARAMS() && matchTokenNew(")")
			&& NT_DECLARATIONS() && NT_STATEMENTSEQ()
			&& matchTokenNew("fed");
	}

	std::cout << "ERROR::FDEC" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_PARAMS()
{
	if (getPeek() == "double" || getPeek() == "int") {
		return NT_TYPE() && NT_VAR() 
			&& NT_PARAMS_EXT();
	}

	std::cout << "ERROR::PARAMS" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_PARAMS_EXT()
{
	if (getPeek() == ",") {
		return matchTokenNew(",") && NT_PARAMS();
	}
	else if (getPeek() == ")") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::PARAMS_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_FNAME()
{
	if (getPeek() == "ID") {
		return matchTokenNew("ID");
	}

	std::cout << "ERROR::FNAME" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_DECLARATIONS()
{
	if (getPeek() == "double" || getPeek() == "int") {
		return NT_DECL() && matchTokenNew(";") 
			&& NT_DECLARATIONS();
	}
	else if (getPeek() == "ID" || getPeek() == "if"
		|| getPeek() == "print" || getPeek() == "return" 
		|| getPeek() == "while") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::DECLARATIONS" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_DECL()
{
	if (getPeek() == "double" || getPeek() == "int") {
		return NT_TYPE() && NT_VARLIST();
	}

	std::cout << "ERROR::DECL" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_TYPE()
{
	if (getPeek() == "double") {
		return matchTokenNew("double");
	}
	else if (getPeek() == "int") {
		return matchTokenNew("int");
	}

	std::cout << "ERROR::TYPE" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_VARLIST()
{
	if (getPeek() == "ID") {
		return NT_VAR() && NT_VARLIST_EXT();
	}

	std::cout << "ERROR::VARLIST" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_VARLIST_EXT()
{
	if (getPeek() == ",") {
		return matchTokenNew(",") && NT_VARLIST();
	}
	else if (getPeek() == ";") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::VARLIST_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_STATEMENT()
{
	if (getPeek() == "ID") {
		return NT_VAR() && matchTokenNew("=")
			&& NT_EXPR();
	}
	else if (getPeek() == "if") {
		return matchTokenNew("if") && NT_BEXPR()
			&& matchTokenNew("then") && NT_STATEMENTSEQ()
			&& NT_STATEMENT_EXT();
	}
	else if (getPeek() == "print") {
		return matchTokenNew("print") && NT_EXPR();
	}
	else if (getPeek() == "return") {
		return matchTokenNew("return") && NT_EXPR();
	}
	else if (getPeek() == "while") {
		return matchTokenNew("while") && NT_BEXPR()
			&& matchTokenNew("do") && NT_STATEMENTSEQ()
			&& matchTokenNew("od");
	}

	std::cout << "ERROR::STATEMENT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_STATEMENT_EXT()
{
	if (getPeek() == "else") {
		return matchTokenNew("else") && NT_STATEMENTSEQ()
			&& matchTokenNew("fi");
	}
	else if (getPeek() == "fi") {
		return matchTokenNew("fi");
	}

	std::cout << "ERROR::STATEMENT_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_STATEMENTSEQ()
{
	if (getPeek() == "ID" || getPeek() == "if"
		|| getPeek() == "print" || getPeek() == "return"
		|| getPeek() == "while") {
		return NT_STATEMENT() && NT_STATEMENTSEQ_EXT();
	}

	std::cout << "ERROR::STATEMENTSEQ" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_STATEMENTSEQ_EXT()
{
	if (getPeek() == ";") {
		return matchTokenNew(";") && NT_STATEMENTSEQ();
	}
	else if (getPeek() == "." || getPeek() == "else"
		|| getPeek() == "fed" || getPeek() == "fi"
		|| getPeek() == "fi" || getPeek() == "od") {
		// EPISLON
		return true;
	}

	std::cout << "ERROR::STATEMENTSEQ_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_EXPR()
{
	if (getPeek() == "(" || getPeek() == "DOUBLE"
		|| getPeek() == "ID" || getPeek() == "INTEGER") {
		return NT_TERM() && NT_EXPR_EXT();
	}

	std::cout << "ERROR::EXPR" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_EXPR_EXT()
{
	if (getPeek() == "+") {
		return matchTokenNew("+") && NT_TERM()
			&& NT_EXPR_EXT();
	}
	else if (getPeek() == "-") {
		return matchTokenNew("-") && NT_TERM()
			&& NT_EXPR_EXT();
	}
	else if (getPeek() == ")" || getPeek() == "]"
		|| getPeek() == "," || getPeek() == "."
		|| getPeek() == "<" || getPeek() == "<="
		|| getPeek() == "<>" || getPeek() == "=="
		|| getPeek() == ">" || getPeek() == ">="
		|| getPeek() == "else" || getPeek() == "fed"
		|| getPeek() == "fi" || getPeek() == "od") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::EXPR_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_TERM()
{
	if (getPeek() == "(" || getPeek() == "DOUBLE" 
		|| getPeek() == "ID" || getPeek() == "INTEGER") {
		return NT_FACTOR() && NT_TERM_EXT();
	}

	std::cout << "ERROR::TERM" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_TERM_EXT()
{
	if (getPeek() == "%") {
		return matchTokenNew("%") && NT_FACTOR()
			&& NT_TERM_EXT();
	}
	else if (getPeek() == "*") {
		return matchTokenNew("*") && NT_FACTOR()
			&& NT_TERM_EXT();
	}
	else if (getPeek() == "/") {
		return matchTokenNew("/") && NT_FACTOR()
			&& NT_TERM_EXT();
	}
	else if (getPeek() == ")" || getPeek() == "+"
		|| getPeek() == "-" || getPeek() == "," || getPeek() == "."
		|| getPeek() == ";" || getPeek() == "<"
		|| getPeek() == "<=" || getPeek() == "<>"
		|| getPeek() == "==" || getPeek() == ">"
		|| getPeek() == ">=" || getPeek() == "else"
		|| getPeek() == "fed" || getPeek() == "fi"
		|| getPeek() == "od") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::TERM_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_FACTOR()
{
	if (getPeek() == "(") {
		return matchTokenNew("(") && NT_EXPR() 
			&& matchTokenNew(")");
	}
	else if (getPeek() == "DOUBLE" || getPeek() == "INTEGER") {
		return NT_NUMBER();
	}
	else if (getPeek() == "ID") {
		return matchTokenNew("ID") && NT_FACTOR_EXT();
	}

	std::cout << "ERROR::FACTOR" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_FACTOR_EXT()
{
	if (getPeek() == "(") {
		return matchTokenNew("(") && NT_EXPRSEQ() && matchTokenNew(")");
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
		return NT_VAR_EXT();
	}

	std::cout << "ERROR::FACTOR_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_EXPRSEQ()
{
	if (getPeek() == "(" || getPeek() == "DOUBLE"
		|| getPeek() == "ID" || getPeek() == "INTEGER") {
		return NT_EXPR() && NT_EXPRSEQ_EXT();
	}

	std::cout << "ERROR::EXPRSEQ" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_EXPRSEQ_EXT()
{
	if (getPeek() == ",") {
		return matchTokenNew(",") && NT_EXPRSEQ();
	}
	else if (getPeek() == ")") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::EXPRSEQ_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_BEXPR()
{
	if (getPeek() == "(" || getPeek() == "not") {
		return NT_BTERM() && NT_BEXPR_EXT();
	}

	std::cout << "ERROR::BEXPR" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_BEXPR_EXT()
{
	if (getPeek() == "or") {
		return matchTokenNew("or") && NT_BTERM()
			&& NT_BEXPR_EXT();
	}
	else if (getPeek() == ")" || getPeek() == "do"
		|| getPeek() == "then") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::BEXPR_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_BTERM()
{
	if (getPeek() == "(" || getPeek() == "not") {
		return NT_BFACTOR() && NT_BTERM_EXT();
	}

	std::cout << "ERROR::BTERM" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_BTERM_EXT()
{
	if (getPeek() == "and") {
		return matchTokenNew("and") && NT_BFACTOR()
			&&NT_BTERM_EXT();
	}
	else if (getPeek() == ")" || getPeek() == "do"
		|| getPeek() == "or" || getPeek() == "then") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::BTERM_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_BFACTOR()
{
	if (getPeek() == "(") {
		return matchTokenNew("(") && NT_BFACTOR_EXT()
			&& matchTokenNew(")");
	}
	else if (getPeek() == "not") {
		return matchTokenNew("not") && NT_BFACTOR();
	}

	std::cout << "ERROR::BFACTOR" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_BFACTOR_EXT()
{
	if (getPeek() == "(") {
		if (NT_BEXPR()) {
			return true;
		}

		if (NT_EXPR()) {
			return true;
		}
	}else if (getPeek() == "DOUBLE" || getPeek() == "ID"
		|| getPeek() == "INTEGER") {
		return NT_EXPR() && NT_COMP()
			&& NT_EXPR();
	}

	std::cout << "ERROR::BFACTOR_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_COMP()
{
	if (getPeek() == "<") {
		return matchTokenNew("<");
	}
	else if (getPeek() == "<=") {
		return matchTokenNew("<=");
	}
	else if (getPeek() == "<>") {
		return matchTokenNew("<>");
	}
	else if (getPeek() == "==") {
		return matchTokenNew("==");
	}
	else if (getPeek() == ">") {
		return matchTokenNew(">");

	}else if (getPeek() == ">=") {
		return matchTokenNew(">=");
	}

	std::cout << "ERROR::COMP" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_VAR()
{
	if (getPeek() == "ID") {
		return matchTokenNew("ID") && NT_VAR_EXT();
	}

	std::cout << "ERROR::VAR" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_VAR_EXT()
{
	if (getPeek() == "[") {
		return matchTokenNew("[") && NT_EXPR()
			&& matchTokenNew("]");
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
		return true;
	}

	std::cout << "ERROR::VAR_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::NT_NUMBER()
{
	if (getPeek() == "DOUBLE") {
		return matchTokenNew("DOUBLE");
	}
	else if (getPeek() == "INTEGER") {
		return matchTokenNew("INTEGER");
	}

	std::cout << "ERROR::NUMBER" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
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
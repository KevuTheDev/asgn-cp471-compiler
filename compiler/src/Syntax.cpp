#include "Syntax.h"

Syntax::Syntax()
{
	this->_position = 0;
}

Syntax::~Syntax()
{
	this->_logFileBuffer = nullptr;
	this->_tokenFileBuffer = nullptr;
	this->_reservedWords = nullptr;
	this->_symbolTable = nullptr;
}

void Syntax::linkLogFileBuffer(LogFileBuffer* buffer)
{
	this->_logFileBuffer = buffer;
}

void Syntax::linkTokenFileBuffer(TokenFileBuffer* buffer)
{
	this->_tokenFileBuffer = buffer;
}

void Syntax::linkReservedWords(ReservedWords* table)
{
	this->_reservedWords = table;
}

void Syntax::linkSymbolTable(SymbolTable* table)
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
		this->_peek = this->_symbolTable->getTokenAtIndex(this->_position);
	}
}

std::string Syntax::getPeek()
{
	return this->_peek;
}

void Syntax::start()
{
	bool hmm = PROGRAM();

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

bool Syntax::PROGRAM()
{
	if (getPeek() == "$" || getPeek() == "."
		|| getPeek() == ";" || getPeek() == "ID"
		|| getPeek() == "def" || getPeek() == "double"
		|| getPeek() == "if" || getPeek() == "int"
		|| getPeek() == "print" || getPeek() == "return"
		|| getPeek() == "while") {
		return FDECLS() && DECLARATIONS()
			&& STATEMESEQ() && matchTokenNew(".");
	}

	std::cout << "ERROR::PROGRAM" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::FDECLS()
{
	if (getPeek() == "def") {
		return FDEC() && matchTokenNew(";")
			&& FDECLS_EXT();
	}
	else if (getPeek() == "." || getPeek() == ";"
		|| getPeek() == "ID" || getPeek() == "def"
		|| getPeek() == "double" || getPeek() == "if"
		|| getPeek() == "int" || getPeek() == "print"
		|| getPeek() == "return" || getPeek() == "while") {
		// EPSILON
		return FDECLS_EXT();
	}

	std::cout << "ERROR::FDECLS" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::FDECLS_EXT()
{
	if (getPeek() == "def") {
		return FDEC() && matchTokenNew(";") && FDECLS_EXT();
	}
	else if (getPeek() == "." || getPeek() == ";"
		|| getPeek() == "ID" || getPeek() == "double"
		|| getPeek() == "if" || getPeek() == "int"
		|| getPeek() == "print" || getPeek() == "return"
		|| getPeek() == "while") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::FDECLS_EXT" << std::endl;
	std::cout << getPeek() << " :: " << readString << std::endl;
	return false;
}

bool Syntax::FDEC()
{
	if (getPeek() == "def") {
		return matchTokenNew("def") && TYPE()
			&& FNAME() && matchTokenNew("(")
			&& PARAMS() && matchTokenNew(")")
			&& DECLARATIONS() && STATEMESEQ()
			&& matchTokenNew("fed");
	}

	std::cout << "ERROR::FDEC" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::PARAMS()
{
	if (getPeek() == "double" || getPeek() == "int") {
		return TYPE() && VAR() 
			&& PARAMS_EXT();
	}

	std::cout << "ERROR::PARAMS" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::PARAMS_EXT()
{
	if (getPeek() == ",") {
		return matchTokenNew(",") && PARAMS();
	}
	else if (getPeek() == ")") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::PARAMS_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::FNAME()
{
	if (getPeek() == "ID") {
		return matchTokenNew("ID");
	}

	std::cout << "ERROR::FNAME" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::DECLARATIONS()
{
	if (getPeek() == "double" || getPeek() == "int") {
		return DECL() && matchTokenNew(";") 
			&& DECLARATIONS_EXT();
	}
	else if (getPeek() == "." || getPeek() == ";"
		|| getPeek() == "ID" || getPeek() == "fed"
		|| getPeek() == "if" || getPeek() == "print" 
		|| getPeek() == "return" || getPeek() == "while") {
		// EPSILON
		return DECLARATIONS_EXT();
	}

	std::cout << "ERROR::DECLARATIONS" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::DECLARATIONS_EXT()
{
		if (getPeek() == "double" || getPeek() == "int") {
			return DECL() && matchTokenNew(";")
				&& DECLARATIONS_EXT();
		}
		else if (getPeek() == "." || getPeek() == ";"
			|| getPeek() == "ID" || getPeek() == "fed"
			|| getPeek() == "if" || getPeek() == "print"
			|| getPeek() == "return" || getPeek() == "while") {
			// EPSILON
			return true;
		}

	std::cout << "ERROR::DECLARATIONS_EXT" << std::endl;
	std::cout << getPeek() << " :: " << readString << std::endl;
	return false;
}

bool Syntax::DECL()
{
	if (getPeek() == "double" || getPeek() == "int") {
		return TYPE() && VARLIST();
	}

	std::cout << "ERROR::DECL" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::TYPE()
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

bool Syntax::VARLIST()
{
	if (getPeek() == "ID") {
		return VAR() && VARLIST_EXT();
	}

	std::cout << "ERROR::VARLIST" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::VARLIST_EXT()
{
	if (getPeek() == ",") {
		return matchTokenNew(",") && VARLIST();
	}
	else if (getPeek() == ";") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::VARLIST_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::STATEMESEQ()
{
	if (getPeek() == ";" || getPeek() == "ID" 
		|| getPeek() == "if" || getPeek() == "print" 
		|| getPeek() == "return" || getPeek() == "while") {
		return STATEMENT() && STATEMESEQ_EXT();
	}
	else if (getPeek() == "." || getPeek() == "else"
		|| getPeek() == "fed" || getPeek() == "fi"
		|| getPeek() == "od") {
		// EPISLON
		return STATEMENT() && STATEMESEQ_EXT();
	}

	std::cout << "ERROR::STATEMENTSEQ" << std::endl;
	std::cout << getPeek() << " :: " << readString << std::endl;
	return false;
}

bool Syntax::STATEMESEQ_EXT()
{
	if (getPeek() == ";") {
		return matchTokenNew(";") && STATEMESEQ();
	}
	else if (getPeek() == "." || getPeek() == "else"
		|| getPeek() == "fed" || getPeek() == "fi"
		|| getPeek() == "od") {
		// EPISLON
		return true;
	}

	std::cout << "ERROR::STATEMENTSEQ_EXT" << std::endl;
	std::cout << getPeek() << " :: " << readString << std::endl;
	return false;
}

bool Syntax::STATEMENT()
{
	if (getPeek() == "ID") {
		return VAR() && matchTokenNew("=")
			&& EXPR();
	}
	else if (getPeek() == "if") {
		return matchTokenNew("if") && BEXPR()
			&& matchTokenNew("then") && STATEMESEQ()
			&& STATEMEEXT();
	}
	else if (getPeek() == "print") {
		return matchTokenNew("print") && EXPR();
	}
	else if (getPeek() == "return") {
		return matchTokenNew("return") && EXPR();
	}
	else if (getPeek() == "while") {
		return matchTokenNew("while") && BEXPR()
			&& matchTokenNew("do") && STATEMESEQ()
			&& matchTokenNew("od");
	}
	else if (getPeek() == "." || getPeek() == ";"
		|| getPeek() == "else" || getPeek() == "fed"
		|| getPeek() == "fi" || getPeek() == "od") {
		return true;
	}

	std::cout << "ERROR::STATEMENT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::STATEMEEXT()
{
	if (getPeek() == "else") {
		return matchTokenNew("else") && STATEMESEQ()
			&& matchTokenNew("fi");
	}
	else if (getPeek() == "fi") {
		return matchTokenNew("fi");
	}

	std::cout << "ERROR::STATEMEEXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::EXPR()
{
	if (getPeek() == "(" || getPeek() == "DOUBLE"
		|| getPeek() == "ID" || getPeek() == "INTEGER") {
		return TERM() && EXPR_EXT();
	}

	std::cout << "ERROR::EXPR" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::EXPR_EXT()
{
	if (getPeek() == "+") {
		return matchTokenNew("+") && TERM()
			&& EXPR_EXT();
	}
	else if (getPeek() == "-") {
		return matchTokenNew("-") && TERM()
			&& EXPR_EXT();
	}
	else if (getPeek() == ")" || getPeek() == "]"
		|| getPeek() == "," || getPeek() == "."
		|| getPeek() == ";"
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

bool Syntax::TERM()
{
	if (getPeek() == "(" || getPeek() == "DOUBLE" 
		|| getPeek() == "ID" || getPeek() == "INTEGER") {
		return FACTOR() && TERM_EXT();
	}

	std::cout << "ERROR::TERM" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::TERM_EXT()
{
	if (getPeek() == "%") {
		return matchTokenNew("%") && FACTOR()
			&& TERM_EXT();
	}
	else if (getPeek() == "*") {
		return matchTokenNew("*") && FACTOR()
			&& TERM_EXT();
	}
	else if (getPeek() == "/") {
		return matchTokenNew("/") && FACTOR()
			&& TERM_EXT();
	}
	else if (
		getPeek() == ")" || getPeek() == "]"
		|| getPeek() == "+" || getPeek() == "-"
		|| getPeek() == "," || getPeek() == "."
		|| getPeek() == ";"
		|| getPeek() == "<" || getPeek() == "<="
		|| getPeek() == "<>" || getPeek() == "=="
		|| getPeek() == ">" || getPeek() == ">="
		|| getPeek() == "else" || getPeek() == "fed"
		|| getPeek() == "fi" || getPeek() == "od") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::TERM_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::FACTOR()
{
	if (getPeek() == "(") {
		return matchTokenNew("(") && EXPR() 
			&& matchTokenNew(")");
	}
	else if (getPeek() == "DOUBLE" || getPeek() == "INTEGER") {
		return NUMBER();
	}
	else if (getPeek() == "ID") {
		return matchTokenNew("ID") && FACTOR_EXT();
	}

	std::cout << "ERROR::FACTOR" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::FACTOR_EXT()
{
	if (getPeek() == "(") {
		return matchTokenNew("(") && EXPRSEQ() && matchTokenNew(")");
	}
	else if (getPeek() == "%" || getPeek() == ")"
		|| getPeek() == "]" || getPeek() == "*"
		|| getPeek() == "/" || getPeek() == "+"
		|| getPeek() == "-" || getPeek() == ","
		|| getPeek() == "." || getPeek() == ";"
		|| getPeek() == "<" || getPeek() == "<="
		|| getPeek() == "<>" || getPeek() == "=="
		|| getPeek() == ">" || getPeek() == ">="
		|| getPeek() == "else" || getPeek() == "fed"
		|| getPeek() == "fi" || getPeek() == "od") {
		return VAR_EXT();
	}

	std::cout << "ERROR::FACTOR_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::EXPRSEQ()
{
	if (getPeek() == "(" || getPeek() == "DOUBLE"
		|| getPeek() == "ID" || getPeek() == "INTEGER") {
		return EXPR() && EXPRSEQ_EXT();
	}
	else if (getPeek() == ")") {
		return true;
	}

	std::cout << "ERROR::EXPRSEQ" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::EXPRSEQ_EXT()
{
	if (getPeek() == ",") {
		return matchTokenNew(",") && EXPRSEQ();
	}
	else if (getPeek() == ")") {
		// EPSILON
		return true;
	}

	std::cout << "ERROR::EXPRSEQ_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::BEXPR()
{
	if (getPeek() == "(" || getPeek() == "not") {
		return BTERM() && BEXPR_EXT();
	}

	std::cout << "ERROR::BEXPR" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::BEXPR_EXT()
{
	if (getPeek() == "or") {
		return matchTokenNew("or") && BTERM()
			&& BEXPR_EXT();
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

bool Syntax::BTERM()
{
	if (getPeek() == "(" || getPeek() == "not") {
		return BFACTOR() && BTERM_EXT();
	}

	std::cout << "ERROR::BTERM" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::BTERM_EXT()
{
	if (getPeek() == "and") {
		return matchTokenNew("and") && BFACTOR()
			&& BTERM_EXT();
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

bool Syntax::BFACTOR()
{
	if (getPeek() == "(") {
		return matchTokenNew("(") && BFACTOR_EXT()
			&& matchTokenNew(")");
	}
	else if (getPeek() == "not") {
		return matchTokenNew("not") && BFACTOR();
	}

	std::cout << "ERROR::BFACTOR" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::BFACTOR_EXT()
{
	if (getPeek() == "(") {
		if (BEXPR()) {
			return true;
		}

		if (EXPR()) {
			return true;
		}
	}
	else if (getPeek() == "not") {
		return BEXPR();
	}
	else if (getPeek() == "DOUBLE" || getPeek() == "ID"
		|| getPeek() == "INTEGER") {
		return EXPR() && COMP()
			&& EXPR();
	}

	std::cout << "ERROR::BFACTOR_EXT" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::COMP()
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

bool Syntax::VAR()
{
	if (getPeek() == "ID") {
		return matchTokenNew("ID") && VAR_EXT();
	}

	std::cout << "ERROR::VAR" << std::endl;
	std::cout << getPeek() << " :: "<< readString << std::endl;
	return false;
}

bool Syntax::VAR_EXT()
{
	if (getPeek() == "[") {
		return matchTokenNew("[") && EXPR()
			&& matchTokenNew("]");
	}
	else if (getPeek() == "%" || getPeek() == ")" || getPeek() == "]"
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

bool Syntax::NUMBER()
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
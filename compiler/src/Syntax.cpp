#include "Syntax.h"

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
	E();
	std::cout << "COMPLETE" << std::endl;
	std::cout << "COMPLETE" << std::endl;
	std::cout << "COMPLETE" << std::endl;
	std::cout << "COMPLETE" << std::endl;
	std::cout << "COMPLETE" << std::endl;
}

void Syntax::matchToken(const std::string& token)
{
	if (token == this->_peek) {
		getNextToken();
	}
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
		this->matchToken("+");
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
		this->matchToken("*");
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
		this->matchToken("id");
	}
	else if (this->_peek == "(") {
		this->matchToken("(");
		E();
		this->matchToken(")");
	}
	else {
		std::cout << "ERROR F" << std::endl;
	}
}

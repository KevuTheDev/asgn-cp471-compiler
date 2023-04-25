#include "Lexical.h"

Lexical::Lexical()
{
	this->_error = false;
}

Lexical::~Lexical()
{
}

void Lexical::linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer)
{
	this->_logFileBuffer = buffer;
}

void Lexical::linkReservedWords(std::shared_ptr<ReservedWords> table)
{
	this->_reservedWords = table;
}

void Lexical::linkTokenList(std::shared_ptr<TokenList> list)
{
	this->_tokenList = list;
}

void Lexical::run()
{
}

bool Lexical::getError()
{
	return false;
}

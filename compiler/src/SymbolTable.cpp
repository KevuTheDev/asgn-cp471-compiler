#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
	this->_printTokenLimit = 0;
	this->_printLexemeLimit = 0;
	this->_printLineNumLimit = 0;
	this->_printCharNumLimit = 0;
}

SymbolTable::~SymbolTable()
{
}

void SymbolTable::linkSymbolTableFileBuffer(std::shared_ptr<SymbolTableFileBuffer> buffer)
{
	this->_symbolTableFileBuffer = buffer;
}

void SymbolTable::print()
{
}

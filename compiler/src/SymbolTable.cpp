#include "SymbolTable.h"

// token, lexeme, line number, char number limits
uint32_t SymbolTable::_printLineNumLimit = 0;
uint32_t SymbolTable::_printCharPosLimit = 0;
uint32_t SymbolTable::_printTokenLimit = 0;
uint32_t SymbolTable::_printLexemeLimit = 0;
uint32_t SymbolTable::_printScopeLimit = 0;
uint32_t SymbolTable::_printTypeLimit = 0;
uint32_t SymbolTable::_printCategoryLimit = 0;

SymbolTable::SymbolTable(std::string tablename)
{
	this->_tableName = tablename;
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

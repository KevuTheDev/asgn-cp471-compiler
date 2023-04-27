#include "SymbolTable.h"

// token, lexeme, line number, char number limits
uint32_t SymbolTable::_printLineNumLimit = 0;
uint32_t SymbolTable::_printCharPosLimit = 0;
//uint32_t SymbolTable::_printTokenLimit = 0;
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

void SymbolTable::append(std::shared_ptr<SymbolTableRow> row)
{
	this->_symbolTable.push_back(row);
}

std::vector<std::shared_ptr<SymbolTable::SymbolTableRow>> SymbolTable::getSymbolTable() const
{
	return this->_symbolTable;
}

uint32_t SymbolTable::getSize()
{
	return (uint32_t) this->_symbolTable.size();
}

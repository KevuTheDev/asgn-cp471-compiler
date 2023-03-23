#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
}

bool SymbolTable::append(std::string token, std::string lexeme, int lineNumber)
{
    SymbolRow yy = { token, lexeme, lineNumber };

    _table.push_back(yy);

    return true;
}

#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
}

bool SymbolTable::append(std::string token, std::string lexeme, int lineNumber)
{
    std::tuple<std::string, std::string, int> yy = { token, lexeme, lineNumber };

    _table.push_back(yy);

    return true;
}

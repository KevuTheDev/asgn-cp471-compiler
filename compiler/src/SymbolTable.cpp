#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
}

bool SymbolTable::append(std::string token, std::string lexeme, int linenumber)
{
    std::tuple<std::string, std::string, int> yy = { token, lexeme, linenumber };

    _table.push_back(yy);

    return true;
}

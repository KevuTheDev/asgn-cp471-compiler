#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
    this->_tableToken = { 16, 5 };
    this->_tableLexeme = { 16, 6 };
    this->_tableLineNum = { 4, 4 };
}

bool SymbolTable::append(std::string token, std::string lexeme, int lineNumber)
{
    SymbolRow yy = { token, lexeme, lineNumber };
    _table.push_back(yy);

    return true;
}

void SymbolTable::printTable()
{
    int attributeNumber = 3; // Number of attribute placment
    int attributePadding = 1; // size of padding between word and vertical line
    int tableVerticalLines = attributeNumber + 1; // number of vertical lines 

    int tableHead = attributeNumber * attributePadding * 2 + tableVerticalLines;
    tableHead += this->_tableToken.limit + this->_tableLexeme.limit +
        this->_tableLineNum.limit;

    char symbol = '=';

    std::string output = "";


    // Print Table Head
    for (auto i = 0; i < tableHead; i++) {
        output += symbol;
    }
    


    std::cout << output << std::endl;

    printf("|%sTOKEN%s|%sLEXEME%s|%sLINE%s|\n", 
        std::string(attributePadding, ' ').c_str(),
        std::string(this->_tableToken.limit - this->_tableToken.length
            + attributePadding, ' ').c_str(),
        std::string(attributePadding, ' ').c_str(),
        std::string(this->_tableLexeme.limit - this->_tableLexeme.length
            + attributePadding, ' ').c_str(),
        std::string(attributePadding, ' ').c_str(),
        std::string(this->_tableLineNum.limit - this->_tableLineNum.length 
            + attributePadding, ' ').c_str());

    std::cout << output << std::endl;




    /*
    ==============================================
    | TOKEN            | LEXEME           | LINE |
    |------------------|------------------|------|
    | IDENTIFIER       | i                | 10   |
    | OPERATOR         | &&               | 13   |
    | ...              | ...              | ...  |
    ==============================================
    
    */
}

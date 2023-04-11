#include "SymbolTable.h"

std::string repeatChar(char ch, int num) {
    if (num == 0) { // base case: when num is 0, return an empty string
        return "";
    }
    else { // recursive case: add the character to the output string and call the function recursively with num-1
        return ch + repeatChar(ch, num - 1);
    }
}

SymbolTable::SymbolTable()
{
    this->_tableToken = { 12, 5 };
    this->_tableLexeme = { 20, 6 };
    this->_tableLineNum = { 4, 4 };

    this->_tokenFileBuffer = nullptr;
}

SymbolTable::~SymbolTable()
{
}

void SymbolTable::linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer)
{
    this->_tokenFileBuffer = buffer;
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
    int tableHeadSize = attributeNumber * attributePadding * 2 + tableVerticalLines;
    tableHeadSize += this->_tableToken.limit + this->_tableLexeme.limit +
        this->_tableLineNum.limit;

    char headSymbol = '=';
    std::string tableHead = repeatChar(headSymbol, tableHeadSize);
    std::string padding = repeatChar(' ', attributePadding);


    // print table head
    std::cout << "PRINT" << std::endl;
    this->_tokenFileBuffer->append(tableHead);
    this->_tokenFileBuffer->append("|" +
        padding +
        "TOKEN" +
        std::string(this->_tableToken.limit - this->_tableToken.length
            + attributePadding, ' ') +
        "|" +
        padding +
        "LEXEME" +
        std::string(this->_tableLexeme.limit - this->_tableLexeme.length
            + attributePadding, ' ') +
        "|" +
        padding +
        "LINE" +
        std::string(this->_tableLineNum.limit - this->_tableLineNum.length
            + attributePadding, ' ') +
        "|"
    );

    this->_tokenFileBuffer->append(tableHead);



    for (auto i : this->_table) {
        this->_tokenFileBuffer->append("|" +
            padding +
            i.token.substr(0, this->_tableToken.limit) +
            std::string(this->_tableToken.limit - i.token.length()
                + attributePadding, ' ') +
            "|" +
            padding + i.lexeme.substr(0, this->_tableLexeme.limit) +
            std::string(this->_tableLexeme.limit - i.lexeme.length()
                + attributePadding, ' ') +
            "|" +
            padding +
            std::to_string(i.lineNumber) +
            std::string(this->_tableLineNum.length - std::to_string(i.lineNumber).length()
                + attributePadding, ' ') +
            "|"
        );
    }

    this->_tokenFileBuffer->append(tableHead);
    this->_tokenFileBuffer->clearBuffer();


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

int SymbolTable::length()
{
    return this->_table.size();
}

std::string SymbolTable::getTokenAtIndex(int index)
{
    SymbolRow sr = this->_table.at(index);
    return sr.token;
}

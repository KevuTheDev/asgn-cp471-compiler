#include "SymbolTable.h"

extern std::unique_ptr<TokenFileBuffer> TOKEN_FILE_BUFFER;

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
}

bool SymbolTable::append(std::string token, std::string lexeme, int lineNumber)
{
    SymbolRow yy = { token, lexeme, lineNumber };
    _table.push_back(yy);

    return true;
}
/*
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
    std::cout << tableHead << std::endl;

    printf("|%sTOKEN%s|%sLEXEME%s|%sLINE%s|\n", 
        padding.c_str(),
        std::string(this->_tableToken.limit - this->_tableToken.length
            + attributePadding, ' ').c_str(),
        padding.c_str(),
        std::string(this->_tableLexeme.limit - this->_tableLexeme.length
            + attributePadding, ' ').c_str(),
        padding.c_str(),
        std::string(this->_tableLineNum.limit - this->_tableLineNum.length 
            + attributePadding, ' ').c_str());

    std::cout << tableHead << std::endl;


    for (auto i : this->_table) {
        printf("|%s%s%s|%s%s%s|%s%d%s|\n",
        padding.c_str(),
        i.token.substr(0, this->_tableToken.limit).c_str(),
        std::string(this->_tableToken.limit - i.token.length()
            + attributePadding, ' ').c_str(),
        padding.c_str(),
        i.lexeme.substr(0, this->_tableLexeme.limit).c_str(),
        std::string(this->_tableLexeme.limit - i.lexeme.length()
            + attributePadding, ' ').c_str(),
        padding.c_str(),
        i.lineNumber,
        std::string(this->_tableLineNum.length - std::to_string(i.lineNumber).length()
            + attributePadding, ' ').c_str()
    );
        //while (iToken.length() > 0 || iLexeme.length() > 0) {

        //    printf("|%s%s%s|%s%s%s|%s%d%s|\n",
        //        padding.c_str(),
        //        iToken.substr(0, this->_tableToken.limit).c_str(),
        //        std::string(this->_tableToken.limit - iToken.length()
        //            + attributePadding, ' ').c_str(),
        //        padding.c_str(),
        //        iLexeme.substr(0, this->_tableLexeme.limit).c_str(),
        //        std::string(this->_tableLexeme.limit - iLexeme.length()
        //            + attributePadding, ' ').c_str(),
        //        padding.c_str(),
        //        i.lineNumber,
        //        std::string(this->_tableLineNum.length - std::to_string(i.lineNumber).length()
        //            + attributePadding, ' ').c_str()
        //    );

        //    if (iToken.length() > this->_tableToken.limit) {
        //        iToken = iToken.substr(this->_tableToken.limit);
        //    }
        //    else {
        //        iToken = "";
        //    }

        //    if (iLexeme.length() > this->_tableLexeme.limit) {
        //        iLexeme = iLexeme.substr(this->_tableLexeme.limit);
        //    }
        //    else {

        //        iLexeme= "";
        //    }
        //}
    }

    std::cout << tableHead << std::endl;


    
    //==============================================
    //| TOKEN            | LEXEME           | LINE |
    //|------------------|------------------|------|
    //| IDENTIFIER       | i                | 10   |
    //| OPERATOR         | &&               | 13   |
    //| ...              | ...              | ...  |
    //==============================================
}
*/

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
    ::TOKEN_FILE_BUFFER->append(tableHead);
    ::TOKEN_FILE_BUFFER->append("|" +
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
            + attributePadding, ' ')
    );

    ::TOKEN_FILE_BUFFER->append(tableHead);



    for (auto i : this->_table) {
        ::TOKEN_FILE_BUFFER->append("|" + 
            padding + 
            i.token.substr(0, this->_tableToken.limit) + 
            std::string(this->_tableToken.limit - i.token.length()
                + attributePadding, ' ') +
            padding + i.lexeme.substr(0, this->_tableLexeme.limit) +
            std::string(this->_tableLexeme.limit - i.lexeme.length()
                + attributePadding, ' ') +
            padding + 
            std::to_string(i.lineNumber) + 
            std::string(this->_tableLineNum.length - std::to_string(i.lineNumber).length()
                + attributePadding, ' ')
        );
    }

    ::TOKEN_FILE_BUFFER->append(tableHead);


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

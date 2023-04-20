#include "SymbolTable.h"

SymbolTable::SymbolTable()
{

    this->_tokenFileBuffer = nullptr;
}

SymbolTable::~SymbolTable()
{
}

void SymbolTable::linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer)
{
    this->_tokenFileBuffer = buffer;
}

bool SymbolTable::append(compiler::TOKEN token, std::string lexeme, int lineNumber, int charNumber)
{
    SymbolRow yy = { token, lexeme, lineNumber, charNumber };
    _table.push_back(yy);

    int tokenLength = compiler::ST_KEYWORDS[token].length();
    int lexemeLength = lexeme.length();
    int lineNumLength = std::to_string(lineNumber).length();
    int charNumLength = std::to_string(charNumber).length();

    this->_printTokenLimit = (tokenLength > this->_printTokenLimit) ? tokenLength : this->_printTokenLimit;
    this->_printLexemeLimit = (lexemeLength > this->_printLexemeLimit) ? lexemeLength : this->_printLexemeLimit;
    this->_printLineNumLimit = (lineNumLength > this->_printLineNumLimit) ? lineNumLength : this->_printLineNumLimit;
    this->_printCharNumLimit = (charNumLength > this->_printCharNumLimit) ? charNumLength : this->_printCharNumLimit;

    return true;
}

void SymbolTable::printTable()
{
    int attributeNumber = 4; // Number of attribute placment
    int attributePadding = 1; // size of padding between word and vertical line
    int tableVerticalLines = attributeNumber + 1; // number of vertical lines 
    int rightPadding = attributePadding + 6;

    int tableHeadSize = attributeNumber * (attributePadding + rightPadding) + tableVerticalLines;

    tableHeadSize += this->_printTokenLimit + this->_printLexemeLimit 
        + this->_printLineNumLimit + this->_printCharNumLimit;


    std::string headLineNum = "LINE";
    std::string headCharNum = "CHAR";
    std::string headToken = "TOKEN";
    std::string headLexeme = "LEXEME";


    std::string tableHead = std::string(tableHeadSize, '=');
    std::string padding = std::string(attributePadding, ' ');

    // print table head
    this->_tokenFileBuffer->append(tableHead);
    std::cout << tableHead << std::endl;

    std::string header = std::format("|{}{}{}|{}{}{}|{}{}{}|{}{}{}|",
        padding, headLineNum, std::string(this->_printLineNumLimit - headLineNum.length()
            + rightPadding, ' '),
        padding, headCharNum, std::string(this->_printCharNumLimit - headCharNum.length()
            + rightPadding, ' '),
        padding, headToken, std::string(this->_printTokenLimit - headToken.length()
            + rightPadding, ' '),
        padding, headLexeme, std::string(this->_printLexemeLimit - headLexeme.length()
            + rightPadding, ' ')
    );

    this->_tokenFileBuffer->append(header);
    std::cout << header << std::endl;

    this->_tokenFileBuffer->append(tableHead);
    std::cout << tableHead << std::endl;



    for (auto i : this->_table) {
        std::string rows = std::format("|{}{}{}|{}{}{}|{}{}{}|{}{}{}|",
            padding, std::to_string(i.lineNumber), std::string(this->_printLineNumLimit - std::to_string(i.lineNumber).length()
                + rightPadding, ' '),
            padding, std::to_string(i.charNumber), std::string(this->_printCharNumLimit - std::to_string(i.charNumber).length()
                + rightPadding, ' '),
            padding, compiler::ST_KEYWORDS[i.token], std::string(this->_printTokenLimit - compiler::ST_KEYWORDS[i.token].length()
                + rightPadding, ' '),
            padding, i.lexeme, std::string(this->_printLexemeLimit - i.lexeme.length()
                + rightPadding, ' ')
        );

        this->_tokenFileBuffer->append(rows);
        std::cout << rows << std::endl;
    }

    this->_tokenFileBuffer->append(tableHead);
    std::cout << tableHead << std::endl;
    this->_tokenFileBuffer->clearBuffer();


    /*
    ========================================================
    | TOKEN          | LEXEME       | LINE     | CHAR      |
    ========================================================
    | int            | int          | 1        | 4         |
    | ID             | a            | 1        | 6         |
    | ,              | ,            | 1        | 6         |
    | ID             | b            | 1        | 8         |
    | ...            | ...          | ...      | ...       |
    ========================================================

    */
}

int SymbolTable::length()
{
    return this->_table.size();
}

compiler::TOKEN SymbolTable::getTokenAtIndex(int index)
{
    SymbolRow sr = this->_table.at(index);
    return sr.token;
}

SymbolRow SymbolTable::getSymbolRowAtIndex(int index)
{
    return this->_table.at(index);
}

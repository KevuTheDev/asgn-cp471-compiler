#include "TokenList.h"

TokenList::TokenList()
{
    this->_printTokenLimit = 0;
    this->_printLexemeLimit = 0;
    this->_printLineNumLimit = 0;
    this->_printCharNumLimit = 0;
}

TokenList::~TokenList()
{
}

void TokenList::linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer)
{
	this->_tokenFileBuffer = buffer;
}

void TokenList::append(uint32_t linenum, uint32_t charpos, compiler::TOKEN token, std::string lexeme)
{
	TokenNode node = { linenum, charpos, token, lexeme };
	this->_tokenList.push_back(node);

    uint32_t tokenLength = (uint32_t) compiler::ST_KEYWORDS[token].length();
    uint32_t lexemeLength = (uint32_t) lexeme.length();
    uint32_t lineNumLength = (uint32_t) std::to_string(linenum).length();
    uint32_t charNumLength = (uint32_t) std::to_string(charpos).length();

    this->_printTokenLimit = (tokenLength > this->_printTokenLimit) ? tokenLength : this->_printTokenLimit;
    this->_printLexemeLimit = (lexemeLength > this->_printLexemeLimit) ? lexemeLength : this->_printLexemeLimit;
    this->_printLineNumLimit = (lineNumLength > this->_printLineNumLimit) ? lineNumLength : this->_printLineNumLimit;
    this->_printCharNumLimit = (charNumLength > this->_printCharNumLimit) ? charNumLength : this->_printCharNumLimit;

}

void TokenList::print()
{
    uint8_t attributeNumber = 4; // Number of attribute placment
    uint8_t attributePadding = 1; // size of padding between word and vertical line
    uint8_t tableVerticalLines = attributeNumber + 1; // number of vertical lines 
    uint8_t rightPadding = attributePadding + 6;

    uint8_t tableHeadSize = attributeNumber * (attributePadding + rightPadding) + tableVerticalLines;

    tableHeadSize += this->_printTokenLimit + this->_printLexemeLimit
        + this->_printLineNumLimit + this->_printCharNumLimit;


    std::string headLineNum = "LINE";
    std::string headCharNum = "CHAR";
    std::string headToken = "TOKEN";
    std::string headLexeme = "LEXEME";


    std::string tableHead = std::string(tableHeadSize, '=');
    std::string padding = std::string(attributePadding, ' ');

    // print table head
    this->_tokenFileBuffer->writeToFile(tableHead);
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

    this->_tokenFileBuffer->writeToFile(header);
    std::cout << header << std::endl;

    this->_tokenFileBuffer->writeToFile(tableHead);
    std::cout << tableHead << std::endl;



    for (auto i : this->_tokenList) {
        std::string rows = std::format("|{}{}{}|{}{}{}|{}{}{}|{}{}{}|",
            padding, std::to_string(i.lineNum), std::string(this->_printLineNumLimit - std::to_string(i.lineNum).length()
                + rightPadding, ' '),
            padding, std::to_string(i.charPos), std::string(this->_printCharNumLimit - std::to_string(i.charPos).length()
                + rightPadding, ' '),
            padding, compiler::ST_KEYWORDS[i.token], std::string(this->_printTokenLimit - compiler::ST_KEYWORDS[i.token].length()
                + rightPadding, ' '),
            padding, i.lexeme, std::string(this->_printLexemeLimit - i.lexeme.length()
                + rightPadding, ' ')
        );

        this->_tokenFileBuffer->writeToFile(rows);
        std::cout << rows << std::endl;
    }

    this->_tokenFileBuffer->writeToFile(tableHead);
    std::cout << tableHead << std::endl;


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

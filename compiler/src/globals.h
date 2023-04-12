#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace compiler
{
    const std::string COMPILER_FILE_EXTENSION_MAIN = ".cp";
    const std::string COMPILER_FILE_EXTENSION_LOG = ".log";
    const std::string COMPILER_FILE_EXTENSION_TOKEN = COMPILER_FILE_EXTENSION_MAIN + "t";
    const std::string COMPILER_FILE_EXTENSION_SYMBOL = COMPILER_FILE_EXTENSION_MAIN + "s";

    const uint8_t COMPILER_FILE_EXTENSION_MAIN_LEN = uint8_t(COMPILER_FILE_EXTENSION_MAIN.length());
    const uint8_t COMPILER_FILE_EXTENSION_LOG_LEN = uint8_t(COMPILER_FILE_EXTENSION_LOG.length());
    const uint8_t COMPILER_FILE_EXTENSION_TOKEN_LEN = uint8_t(COMPILER_FILE_EXTENSION_TOKEN.length());
    const uint8_t COMPILER_FILE_EXTENSION_SYMBOL_LEN = uint8_t(COMPILER_FILE_EXTENSION_SYMBOL.length());

    const uint16_t COMPILER_BUFFER_SIZE = 2048;
    const uint16_t COMPILER_BUFFER_SIZE_NULL = COMPILER_BUFFER_SIZE - 1;

    const std::vector<std::string> KEYWORDS = { "def", "fed", "int", "double", "if", 
        "then", "else", "fi", "while", "do", "od", "print", "return", "or", "and", "not" };

    const std::string TK_END = "$";
    const std::string TK_MODULU = "%";
    const std::string TK_LSEMIC = "(";
    const std::string TK_RSEMIC = ")";
    const std::string TK_LBRACK = "[";
    const std::string TK_RBRACK = "]";
    const std::string TK_MULTIP = "*";
    const std::string TK_DIVIDE = "/";
    const std::string TK_ADDITI = "+";
    const std::string TK_MINUS = "-";




    enum Symbol {
        TOKEN,
        LEXEME,
        LINE
    };

    enum TOKEN {
        SEMICOLON = ';',
        LPAREN = '(',
        RPAREN = ')',
        AMPERS = '&',



        COMPARISON,
        IDENTIFIER,
        NUMBER,

    };


    // Function methods

    inline void readFileToBuffers(std::ifstream& is, char* buf)
    {
        if (is.is_open()) {
            is.read(buf, compiler::COMPILER_BUFFER_SIZE_NULL);
            return;
        }
    }

    inline void writeToFile(std::ofstream& os, std::string buffer)
    {
        if (os.is_open()) {
            os << buffer;
        }
    }
}
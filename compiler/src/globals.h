#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

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

    const std::string KEYWORDS[] = {"def", "fed", "int", "double", "if",
        "then", "else", "fi", "while", "do", "od", "print", "return", "or", "and", "not" };
    // adding keyword to vector, must also add keyword to TOKEN ENUM
    
    const std::string ST_KEYWORDS[] = {"KW_DEF",
        "KW_FED",
        "KW_INT",
        "KW_DOUBLE",
        "KW_IF",
        "KW_THEN",
        "KW_ELSE",
        "KW_FI",
        "KW_WHILE",
        "KW_DO",
        "KW_OD",
        "KW_PRINT",
        "KW_RETURN",
        "KW_OR",
        "KW_AND",
        "KW_NOT",

        "SEMICOLON",
        "LEFT_PAREN",
        "RIGHT_PAREN",
        "EQUAL",
        "COMP_EQUAL",
        "COMP_LEQUAL",
        "COMP_NOT",
        "COMP_LTHAN",
        "COMP_GEQUAL",
        "COMP_GTHAN",
        "COMMA",
        "PLUS",
        "MULTIPLY",
        "MINUS",
        "MODULUS",
        "DIVIDE",
        "LEFT_BRACK",
        "RIGHT_BRACK",
        "DOT",
        "DOLLAR_SIGN",

        "ID",
        "VALUE_INTEGER",
        "VALUE_DOUBLE" };

    enum TOKEN {
        KW_DEF,
        KW_FED,
        KW_INT,
        KW_DOUBLE,
        KW_IF,
        KW_THEN,
        KW_ELSE,
        KW_FI,
        KW_WHILE,
        KW_DO,
        KW_OD,
        KW_PRINT,
        KW_RETURN,
        KW_OR,
        KW_AND,
        KW_NOT,

        SEMICOLON,
        LEFT_PAREN,
        RIGHT_PAREN,
        EQUAL,
        COMP_EQUAL,
        COMP_LEQUAL,
        COMP_NOT,
        COMP_LTHAN,
        COMP_GEQUAL,
        COMP_GTHAN,
        COMMA,
        PLUS,
        MULTIPLY,
        MINUS,
        MODULUS,
        DIVIDE,
        LEFT_BRACK,
        RIGHT_BRACK,
        DOT,
        DOLLAR_SIGN,

        ID,
        VALUE_INTEGER,
        VALUE_DOUBLE

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

    inline void printConsoleError(const std::string& errorMsg)
    {

    }

    inline void printConsoleWarning(const std::string& warningMsg)
    {

    }

    inline void printConsoleInfo(const std::string& infoMsg)
    {

    }
}
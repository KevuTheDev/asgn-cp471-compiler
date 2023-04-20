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

    const std::string ERRORMSG = "[Compiler";

    const std::string KEYWORDS[] = {"def", "fed", "int", "double", "if",
        "then", "else", "fi", "while", "do", "od", "print", "return", "or", "and", "not" };
    // adding keyword to vector, must also add keyword to TOKEN ENUM
    
    const std::string ST_KEYWORDS[] = {
        "KW_DEF",
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

    enum COMPILER_STAGE {
        COMPILER,
        LEXICAL,
        SYNTAX,
        SEMANTIC,
        INTERMEDIATE,
        SYMBOLTABLE,
        LOGFILEBUFFER,
        TOKENFILEBUFFER,
    };

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

    inline std::string getCurrentTimestamp()
    {
        auto tp = std::chrono::zoned_time{ std::chrono::current_zone(),
                                            std::chrono::system_clock::now() }.get_local_time();
        auto dp = std::chrono::floor<std::chrono::days>(tp);
        std::chrono::hh_mm_ss time{ std::chrono::floor<std::chrono::milliseconds>(tp - dp) };
        auto h = time.hours();
        auto M = time.minutes();
        auto s = time.seconds();

        std::string hh = std::to_string(h.count());
        hh = (hh.length() != 2) ? "0" + hh : hh;

        std::string MM = std::to_string(M.count());
        MM = (MM.length() != 2) ? "0" + MM : MM;

        std::string ss = std::to_string(s.count());
        ss = (ss.length() != 2) ? "0" + ss : ss;

        std::string output = std::format("[{}:{}:{}]",
            hh, MM, ss);

        return output;
    }

    inline std::string getCompilerStageString(COMPILER_STAGE stage)
    {

        switch (stage)
        {
        case compiler::COMPILER:
            return "____COMPILER";
            break;
        case compiler::LEXICAL:
            return "   LEXICAL  ";
            break;
        case compiler::SYNTAX:
            return "   SYNTAX   ";
            break;
        case compiler::SEMANTIC:
            return "  SEMANTIC  ";
            break;
        case compiler::INTERMEDIATE:
            return "INTERMEDIATE";
            break;
        case compiler::SYMBOLTABLE:
            return "SYMBOL TABLE";
            break;
        case compiler::LOGFILEBUFFER:
            return " LOG BUFFER ";
            break;
        case compiler::TOKENFILEBUFFER:
            return "TOKEN BUFFER";
            break;
        default:
            break;
        }
    }

    inline std::string getConsoleError(COMPILER_STAGE stage, const std::string& errorMsg)
    {
        std::string tag = getCompilerStageString(stage);
        std::string timestamp = getCurrentTimestamp();
        return timestamp + " [" + tag + " /ERRO]: " + errorMsg;
    }

    inline std::string getConsoleWarning(COMPILER_STAGE stage, const std::string& warningMsg)
    {
        std::string tag = getCompilerStageString(stage);
        std::string timestamp = getCurrentTimestamp();
        return timestamp + " [" + tag + " /WARN]: " + warningMsg;
    }

    inline std::string getConsoleInfo(COMPILER_STAGE stage, const std::string& infoMsg)
    {
        std::string tag = getCompilerStageString(stage);
        std::string timestamp = getCurrentTimestamp();
        return timestamp + " [" + tag + " /INFO]: " + infoMsg;
    }

    inline void printConsoleError(COMPILER_STAGE stage, const std::string& errorMsg)
    {
        std::string error = getConsoleError(stage, errorMsg);
        std::cout << error << std::endl;
    }

    inline void printConsoleWarning(COMPILER_STAGE stage, const std::string& warningMsg)
    {
        std::string warning = getConsoleWarning(stage, warningMsg);
        std::cout << warning << std::endl;
    }

    inline void printConsoleInfo(COMPILER_STAGE stage, const std::string& infoMsg)
    {
        std::string info = getConsoleInfo(stage, infoMsg);
        std::cout << info << std::endl;
    }
}
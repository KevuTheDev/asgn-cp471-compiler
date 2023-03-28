#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "globals.h"
#include "Lexical.h"
#include "Syntax.h"

#include "ReservedWords.h"
#include "SymbolTable.h"
#include "LogFileBuffer.h"
#include "TokenFileBuffer.h"

std::unique_ptr<Lexical> LEXICAL;
std::unique_ptr<Syntax> SYNTAX;
std::unique_ptr<ReservedWords> RESERVED_WORDS;
std::unique_ptr<SymbolTable> SYMBOL_TABLE;
std::unique_ptr<LogFileBuffer> LOG_FILE_BUFFER;
std::unique_ptr<TokenFileBuffer> TOKEN_FILE_BUFFER;

int main(int argc, char* argv[]) 
{
    std::string filename = "e1";

    ::LOG_FILE_BUFFER = std::make_unique<LogFileBuffer>("output/" + filename + compiler::COMPILER_FILE_EXTENSION_LOG);
    ::TOKEN_FILE_BUFFER = std::make_unique<TokenFileBuffer>("output/" + filename + compiler::COMPILER_FILE_EXTENSION_TOKEN);

    ::RESERVED_WORDS = std::make_unique<ReservedWords>();
    ::SYMBOL_TABLE = std::make_unique<SymbolTable>();

    ::LEXICAL = std::make_unique<Lexical>("res/" + filename + compiler::COMPILER_FILE_EXTENSION_MAIN);


    ::RESERVED_WORDS->addReservedWord("def");
    ::RESERVED_WORDS->addReservedWord("fed");
    ::RESERVED_WORDS->addReservedWord("int");
    ::RESERVED_WORDS->addReservedWord("double");
    ::RESERVED_WORDS->addReservedWord("if");
    ::RESERVED_WORDS->addReservedWord("then");
    ::RESERVED_WORDS->addReservedWord("else");
    ::RESERVED_WORDS->addReservedWord("fi");
    ::RESERVED_WORDS->addReservedWord("while");
    ::RESERVED_WORDS->addReservedWord("do");
    ::RESERVED_WORDS->addReservedWord("od");
    ::RESERVED_WORDS->addReservedWord("print");
    ::RESERVED_WORDS->addReservedWord("return");
    ::RESERVED_WORDS->addReservedWord("or");
    ::RESERVED_WORDS->addReservedWord("and");
    ::RESERVED_WORDS->addReservedWord("not");


    ::LEXICAL->run();


    ::SYNTAX = std::make_unique<Syntax>();
    ::SYNTAX->start();



    return 0;
}
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "globals.h"
#include "Lexical.h"
#include "SymbolTable.h"
#include "LogFileBuffer.h"
#include "TokenFileBuffer.h"

std::unique_ptr<Lexical> LEXICAL;
std::unique_ptr<SymbolTable> SYMBOL_TABLE;
std::unique_ptr<LogFileBuffer> LOG_FILE_BUFFER;
std::unique_ptr<TokenFileBuffer> TOKEN_FILE_BUFFER;

int main(int argc, char* argv[]) 
{
    std::string filename = "Test3";



    ::LEXICAL = std::make_unique<Lexical>("res/" + filename + compiler::COMPILER_FILE_EXTENSION_MAIN);
    ::SYMBOL_TABLE = std::make_unique<SymbolTable>();
    ::LOG_FILE_BUFFER = std::make_unique<LogFileBuffer>("output/" + filename + compiler::COMPILER_FILE_EXTENSION_LOG);
    ::TOKEN_FILE_BUFFER = std::make_unique<TokenFileBuffer>("output/" + filename + compiler::COMPILER_FILE_EXTENSION_TOKEN);

    //SymbolTable st = SymbolTable();

    //lex03->linkSymbolTable(st);
    ::LEXICAL->run();

    // std::cout << compiler::TOKEN::SEMICOLON << std::endl;




    return 0;
}
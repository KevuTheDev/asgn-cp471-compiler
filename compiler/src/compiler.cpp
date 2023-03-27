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
    //Lexical lex00 = Lexical("res/Test0.cp");   // Peak 614 MB
    //Lexical lex01 = Lexical("res/Test1.cp");
    //Lexical lex02 = Lexical("res/Test2.cp");
    //Lexical lex03 = Lexical("res/Test3.cp");
    //Lexical lex04 = Lexical("res/Test4.cp");
    //Lexical lex05 = Lexical("res/Test5.cp");
    //Lexical lex06 = Lexical("res/Test6.cp");
    //Lexical lex07 = Lexical("res/Test7.cp");
    //Lexical lex08 = Lexical("res/Test8.cp");
    //Lexical lex09 = Lexical("res/Test9.cp");

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
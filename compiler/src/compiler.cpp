#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "Lexical.h"
#include "SymbolTable.h"

int main(int argc, char* argv[]) 
{
    //Lexical lex00 = Lexical("res/Test0.cp");   // Peak 614 MB
    //Lexical lex01 = Lexical("res/Test1.cp");
    //Lexical lex02 = Lexical("res/Test2.cp");
    Lexical lex03 = Lexical("res/Test3.cp");
    //Lexical lex04 = Lexical("res/Test4.cp");
    //Lexical lex05 = Lexical("res/Test5.cp");
    //Lexical lex06 = Lexical("res/Test6.cp");
    //Lexical lex07 = Lexical("res/Test7.cp");
    //Lexical lex08 = Lexical("res/Test8.cp");
    //Lexical lex09 = Lexical("res/Test9.cp");

    SymbolTable st = SymbolTable();

    lex03.linkSymbolTable(st);
    lex03.run();





    // std::cout << compiler::TOKEN::SEMICOLON << std::endl;


    return 0;
}
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "globals.h"

#include "Compiler.h"

#include "Lexical.h"
#include "Syntax.h"

#include "ReservedWords.h"
#include "SymbolTable.h"
#include "LogFileBuffer.h"
#include "TokenFileBuffer.h"

int main(int argc, char* argv[]) 
{
    std::string filename = "Test2";
    std::string outpath = "output/";
    std::string respath = "res/";

    auto compiler = std::make_unique<Compiler>(filename, outpath, respath);
    compiler->run();
    

    return 0;
}
#include <iostream>
#include <fstream>
#include "Lexical.h"

int main(int argc, char* argv[]) 
{
    //std::cout << "READING FILE" << std::endl;
    //std::cout << "==========================================" << std::endl;


    //std::ifstream file("res/Test1.cp");
    //char c;

    //if (!file.is_open()) {
    //    std::cout << "Failed to open file" << std::endl;
    //    return 1;
    //}

    //while (file.get(c)) {
    //    std::cout << c;
    //}
    //std::cout << std::endl;
    //std::cout << "==========================================" << std::endl;
    //std::cout << c << std::endl;

    //file.close();
    //std::cout << argv[0] << std::endl;
    //Lexical lex0 = Lexical("res/Test0.cp");
    Lexical lex1 = Lexical("res/Test1.cp");
    Lexical lex2 = Lexical("res/Test2.cp");
    Lexical lex3 = Lexical("res/Test3.cp");
    Lexical lex4 = Lexical("res/Test4.cp");
    Lexical lex5 = Lexical("res/Test5.cp");
    Lexical lex6 = Lexical("res/Test6.cp");
    Lexical lex7 = Lexical("res/Test7.cp");
    Lexical lex8 = Lexical("res/Test8.cp");
    Lexical lex9 = Lexical("res/Test9.cp");


    return 0;
}


// !!!make the lexical analyzer to scan the whole file
// 

/*
    Main loop, read till end of file

    scan for lexemes,
        Must have a way to peak
    
*/
#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "Lexical.h"

#include <thread>

using namespace std::chrono_literals;

void process(int num) {

    for (auto i = 0; i < num; i++) {
        std::this_thread::sleep_for(1000ms);
    }
}

void readFileLine(std::ifstream &is, char* buf) {
    is.read(buf, compiler::COMPILER_BUFFER_SIZE_NULL);

    return;
}


int main(int argc, char* argv[]) 
{
    std::cout << "READING FILE" << std::endl;
    std::cout << "==========================================" << std::endl;


    std::ifstream file("res/Test0.cp", std::ifstream::in);

    if (!file.is_open()) {
        std::cout << "Failed to open file" << std::endl;
        return 1;
    }

    char* buffer1 = new char[compiler::COMPILER_BUFFER_SIZE];
    char* buffer2 = new char[compiler::COMPILER_BUFFER_SIZE];

    memset(buffer1, 0, compiler::COMPILER_BUFFER_SIZE);
    memset(buffer2, 0, compiler::COMPILER_BUFFER_SIZE);

    while (true) {
        auto helper1 = std::thread(readFileLine, std::ref(file), std::ref(buffer1));
        helper1.join();
        std::cout << buffer1;
        
        if (file.gcount() == 0) {
            break;
        }


        auto helper2 = std::thread(readFileLine, std::ref(file), std::ref(buffer2));
        helper2.join();
        std::cout << buffer2;
    }






    std::cout << std::endl;
    std::cout << "==========================================" << std::endl;

    if (file)
        std::cout << "all characters read successfully.";
    else
        std::cout << "error: only " << file.gcount() << " could be read";


    file.close();

    //Lexical lex1 = Lexical("res/Test1.cp");


    return 0;
}


// !!!make the lexical analyzer to scan the whole file
// 

/*
    Main loop, read till end of file

    scan for lexemes,
        Must have a way to peak
    
*/
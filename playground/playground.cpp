#include <iostream>
#include <fstream>
#include <locale>
#include <string>


int main() {
    std::locale::global(std::locale("en_US.UTF-8"));
    std::ios_base::sync_with_stdio(false);
    std::wcin.imbue(std::locale("en_US.UTF-8"));
    std::wcout.imbue(std::locale("en_US.UTF-8"));

    std::ifstream input_file("input_file.txt");
    std::string line;
    if (input_file.is_open()) {
        while (std::getline(input_file, line)) {
            std::cout << line << std::endl;
        }
        input_file.close();
    }
    else {
        std::cout << "Unable to open input file." << std::endl;
        return 1;
    }

    std::string substring = "ϵ";
    std::size_t position = line.find(substring);
    std::cout << position << std::endl;
    std::cout << substring << std::endl;

    std::ofstream output_file("output_file.txt");
    if (output_file.is_open()) {
        std::string utf8_string = line;
        output_file << utf8_string << std::endl;
        output_file.close();
    }
    else {
        std::cout << "Unable to open output file." << std::endl;
        return 1;
    }

    return 0;
}
#pragma once
#include <iostream>
#include <string>

namespace compiler
{
    const std::string COMPILER_FILE_EXTENSION = ".cp";
    const uint8_t COMPILER_FILE_EXTENSION_LEN = 3;
    const uint16_t COMPILER_BUFFER_SIZE = 2048;
    const uint16_t COMPILER_BUFFER_SIZE_NULL = COMPILER_BUFFER_SIZE - 1;
}
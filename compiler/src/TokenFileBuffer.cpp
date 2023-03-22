#include "TokenFileBuffer.h"

bool TokenFileBuffer::checkExtension(const std::string& filepath)
{
	if (filepath.length() < compiler::COMPILER_FILE_EXTENSION_TOKEN_LEN + 1) {
		std::cout << "TOKEN | Invalid file string specified: length size" << std::endl;
		return false;
	}

	// check if directory or file
	if (filepath.substr(filepath.length() - compiler::COMPILER_FILE_EXTENSION_TOKEN_LEN)
		!= compiler::COMPILER_FILE_EXTENSION_TOKEN) {
		std::cout << "TOKEN | Invalid file string specified: file extension" << std::endl;
		return false;
	}

	return true;
}

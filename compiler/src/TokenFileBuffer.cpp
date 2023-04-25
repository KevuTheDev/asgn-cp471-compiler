#include "TokenFileBuffer.h"

bool TokenFileBuffer::checkExtension(const std::string& filepath)
{
	if (filepath.length() < compiler::COMPILER_FILE_EXTENSION_TOKEN_LEN + 1) {
		compiler::printConsoleError(compiler::TOKENFILEBUFFER, "Invalid file string specified: length size");
		return false;
	}

	// check if directory or file
	if (filepath.substr(filepath.length() - compiler::COMPILER_FILE_EXTENSION_TOKEN_LEN)
		!= compiler::COMPILER_FILE_EXTENSION_TOKEN) {
		compiler::printConsoleError(compiler::TOKENFILEBUFFER, "Invalid file string specified: file extension");
		return false;
	}

	return true;
}


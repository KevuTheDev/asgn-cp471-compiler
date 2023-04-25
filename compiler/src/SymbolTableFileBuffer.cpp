#include "SymbolTableFileBuffer.h"

bool SymbolTableFileBuffer::checkExtension(const std::string& filepath)
{
	if (filepath.length() < compiler::COMPILER_FILE_EXTENSION_SYMBOL_LEN + 1) {
		compiler::printConsoleError(compiler::SYMBOLTABLEFILEBUFFER, "Invalid file string specified: length size");
		return false;
	}

	// check if directory or file
	if (filepath.substr(filepath.length() - compiler::COMPILER_FILE_EXTENSION_SYMBOL_LEN)
		!= compiler::COMPILER_FILE_EXTENSION_SYMBOL) {
		compiler::printConsoleError(compiler::SYMBOLTABLEFILEBUFFER, "Invalid file string specified: file extension");
		return false;
	}

	return true;
}

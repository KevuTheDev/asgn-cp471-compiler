#include "LogFileBuffer.h"

bool LogFileBuffer::checkExtension(const std::string& filepath)
{
	if (filepath.length() < compiler::COMPILER_FILE_EXTENSION_LOG_LEN + 1) {
		compiler::printConsoleError(compiler::LOGFILEBUFFER, "Invalid file string specified: length size");
		return false;
	}

	// check if directory or file
	if (filepath.substr(filepath.length() - compiler::COMPILER_FILE_EXTENSION_LOG_LEN)
		!= compiler::COMPILER_FILE_EXTENSION_LOG) {
		compiler::printConsoleError(compiler::LOGFILEBUFFER, "Invalid file string specified: file extension");
		return false;
	}

	return true;
}

void LogFileBuffer::logLexicalError(uint64_t linenumber, uint64_t charposition, const std::string& error, const std::string& currentline)
{
	std::string err = compiler::getConsoleError(compiler::LEXICAL, "Error on line " + std::to_string(linenumber) +
		":" + std::to_string(charposition) + " | Invalid character \'" + error + "\'");
	// Error on line 20: Invalid character
	// void FileBuffer:writeToFile(~
	//                     ~~~^
	this->writeToFile(err);
	this->writeToFile(compiler::getConsoleError(compiler::LEXICAL, currentline));

	std::string line(charposition - 1, '~');

	this->writeToFile(compiler::getConsoleError(compiler::LEXICAL, line + "^"));

	
}

void LogFileBuffer::logSyntaxError(uint64_t linenumber, uint64_t charposition, const std::string& error)
{
	std::string err = compiler::getConsoleError(compiler::LEXICAL, "Error on line " + std::to_string(linenumber) +
		":" + std::to_string(charposition) + " | Invalid character \'" + error + "\'");
	this->writeToFile(err);
}

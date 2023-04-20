#include "LogFileBuffer.h"

LogFileBuffer::~LogFileBuffer()
{
	this->finish();
}

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

void LogFileBuffer::logLexicalError(uint32_t linenumber, uint32_t rownumber, const std::string& errorchar)
{
	std::string err = compiler::getConsoleError(compiler::LEXICAL, "Error on line " + std::to_string(linenumber) +
		":" + std::to_string(rownumber) + " | Invalid character \'" + errorchar + "\'");
	// Error on line 20: Invalid character
	// void FileBuffer:append(~
	//                     ~~~^
	this->append(err);
}

void LogFileBuffer::logLexicalErrorNew(uint32_t linenumber, uint32_t rownumber, const std::string& errorchar, const std::string& currentLine)
{
	std::string err = compiler::getConsoleError(compiler::LEXICAL, "Error on line " + std::to_string(linenumber) +
		":" + std::to_string(rownumber) + " | Invalid character \'" + errorchar + "\'");
	// Error on line 20: Invalid character
	// void FileBuffer:append(~
	//                     ~~~^
	this->append(err);
	this->append(compiler::getConsoleError(compiler::LEXICAL, currentLine));

	std::string line(rownumber - 1, '~');

	this->append(compiler::getConsoleError(compiler::LEXICAL, line + "^"));

	
}

void LogFileBuffer::logSyntaxError(uint32_t linenumber, uint32_t rownumber, const std::string& errorchar)
{
	std::string err = compiler::getConsoleError(compiler::LEXICAL, "Error on line " + std::to_string(linenumber) +
		":" + std::to_string(rownumber) + " | Invalid character \'" + errorchar + "\'");
	this->append(err);
}

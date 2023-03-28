#include "LogFileBuffer.h"

LogFileBuffer::~LogFileBuffer()
{
	this->finish();
}

bool LogFileBuffer::checkExtension(const std::string& filepath)
{
	if (filepath.length() < compiler::COMPILER_FILE_EXTENSION_LOG_LEN + 1) {
		std::cout << "LOG | Invalid file string specified: length size" << std::endl;
		return false;
	}

	// check if directory or file
	if (filepath.substr(filepath.length() - compiler::COMPILER_FILE_EXTENSION_LOG_LEN)
		!= compiler::COMPILER_FILE_EXTENSION_LOG) {
		std::cout << "LOG | Invalid file string specified: file extension" << std::endl;
		return false;
	}

	return true;
}

void LogFileBuffer::errorChar(int linenumber, int rownumber, const std::string& errorchar)
{
	std::string e = "Error on line " + std::to_string(linenumber) +
		":" + std::to_string(rownumber) + " | Invalid character \'" + errorchar + "\'";
	// Error on line 20: Invalid character
	// void FileBuffer:append(~
	//                     ~~~^
	this->append(e);
}

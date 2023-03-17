#include "TokenFileBuffer.h"

TokenFileBuffer::TokenFileBuffer()
{
}

TokenFileBuffer::TokenFileBuffer(const std::string& filename)
{
	if (!this->checkExtension(filename)) {
		return;
	}

	this->_fileName = filename;

	this->_os = std::ofstream(this->_fileName);
	if (!this->_os.is_open()) {
		std::cout << "Failed to open file" << std::endl;
		return;
	}

}

void TokenFileBuffer::append(const std::string& value)
{
	this->_buffer += value;

	if (this->_buffer.length() >= this->_bufferSize) {
		// write to the file
		//
		//
		//
		this->_os << this->_buffer;

		this->_buffer = "";
	}
}

void TokenFileBuffer::append(const char* value)
{
	std::string s(value);

	this->_buffer += s;
	if (this->_buffer.length() >= this->_bufferSize) {
		// write to the file
		//
		//
		//
		this->_os << this->_buffer;

		this->_buffer = "";
	}
}

bool TokenFileBuffer::checkExtension(const std::string& filepath)
{

	if (filepath.length() < compiler::COMPILER_FILE_EXTENSION_TOKEN_LEN + 1) {
		std::cout << "Invalid file string specified: length size" << std::endl;
		return false;
	}

	// check if directory or file

	if (filepath.substr(filepath.length() - compiler::COMPILER_FILE_EXTENSION_TOKEN_LEN) 
		!= compiler::COMPILER_FILE_EXTENSION_TOKEN) {
		std::cout << "Invalid file string specified: file extension" << std::endl;
		return false;
	}

	return true;
}
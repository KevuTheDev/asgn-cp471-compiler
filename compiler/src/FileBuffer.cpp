#include "FileBuffer.h"

void writeToFile(std::ofstream& os, std::string buffer)
{
	os << buffer;
}

FileBuffer::FileBuffer()
{
}

FileBuffer::FileBuffer(const std::string& filename)
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

void FileBuffer::append(const std::string& value)
{
	this->_buffer += value + "\n";

	//if (this->_buffer.length() >= this->_bufferSize) {
	//	// write to the file
	//	//
	//	//
	//	//
	//	writeToFile(this->_os, this->_buffer);
	//	//auto h = std::thread(writeToFile, std::ref(this->_os), this->_buffer);
	//	//h.join();

	//	this->_buffer = "";
	//}
}

void FileBuffer::append(const char* value)
{
	std::string s(value);
	this->append(s);
}

bool FileBuffer::checkExtension(const std::string& filepath)
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

void FileBuffer::finish()
{
	this->_os << this->_buffer;
	this->_os.close();
}

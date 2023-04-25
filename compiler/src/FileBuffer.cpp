#include "FileBuffer.h"

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

FileBuffer::~FileBuffer()
{
	this->close();
}

void FileBuffer::writeToFile(const std::string& value)
{
	if (this->_os.is_open()) {
		this->_os << value + "\n";
	}
}

void FileBuffer::writeToFile(const char* value)
{
	std::string s(value);
	this->writeToFile(s);
}

bool FileBuffer::checkExtension(const std::string& filepath)
{
	return true;
}

void FileBuffer::close()
{
	this->_os.close();
}
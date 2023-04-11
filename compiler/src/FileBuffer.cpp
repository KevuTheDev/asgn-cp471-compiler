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

FileBuffer::FileBuffer()
{
}

FileBuffer::~FileBuffer()
{
	this->finish();
	std::cout << "FILEBUFFER DESCTURCTOR CALLED" << std::endl;
}

void FileBuffer::append(const std::string& value)
{
	std::cout << value << std::endl;
	this->_buffer += value + "\n";
	if (this->_buffer.length() >= this->_bufferSizeLimit) {
		// write to the file
		compiler::writeToFile(this->_os, this->_buffer);

		//auto h = std::thread(writeToFile, std::ref(this->_os), this->_buffer);
		//h.join();

		this->_buffer = "";
	}
}

void FileBuffer::append(const char* value)
{
	std::string s(value);
	this->append(s);
}

bool FileBuffer::checkExtension(const std::string& filepath)
{
	return true;
}

void FileBuffer::clearBuffer()
{
	compiler::writeToFile(this->_os, this->_buffer);
	this->_buffer = "";
}

void FileBuffer::close()
{
	this->_os.close();
}

void FileBuffer::finish()
{
	this->_os << this->_buffer;
	this->_buffer = "";
	this->_os.close();
}

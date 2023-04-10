#include "Compiler.h"

Compiler::Compiler(const std::string& filename)
{
	this->_filename = filename;

	LogFileBuffer l = LogFileBuffer(filename + compiler::COMPILER_FILE_EXTENSION_LOG);

	this->_logfilebuffer = &l;


}

void Compiler::run()
{
	if (this->_logfilebuffer != nullptr) {
		std::cout << "HELLO WORLD" << std::endl;
	}
	else {

		std::cout << "HELLO WORLD2222" << std::endl;
	}
}



Compiler::~Compiler()
{
}

#include "Compiler.h"

Compiler::Compiler(const std::string& filename, const std::string& outpath, const std::string& respath)
{
	compiler::printConsoleInfo(compiler::COMPILER, "Starting compiler...");
	this->_filename = filename;
	this->_outpath = outpath;
	this->_respath = respath;
	compiler::printConsoleInfo(compiler::COMPILER, "Compiling " + this->_filename + compiler::COMPILER_FILE_EXTENSION_MAIN + "...");
}

void Compiler::run()
{
}

void Compiler::setupReservedWordsTable()
{
}

void Compiler::addReservedWords()
{
}

void Compiler::setupSymbolTable()
{
}

void Compiler::printSymbolTable()
{
}

void Compiler::setupLexicalAnalysis()
{
}

void Compiler::runLexicalAnalysis()
{
}

void Compiler::setupSyntaxAnalysis()
{
}

void Compiler::runSyntaxAnalysis()
{
}

void Compiler::setupSemanticAnalysis()
{
}

void Compiler::runSemanticAnalysis()
{
}

void Compiler::setupICRGeneration()
{
}

void Compiler::runICRGeneration()
{
}

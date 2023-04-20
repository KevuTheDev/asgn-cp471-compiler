#include "Compiler.h"

Compiler::Compiler(const std::string& filename, const std::string& outpath, const std::string& respath)
{
	compiler::printConsoleInfo(compiler::COMPILER, "Starting compiler...");
	this->_filename = filename;
	this->_outpath = outpath;
	this->_respath = respath;
	compiler::printConsoleInfo(compiler::COMPILER, "Compiling " + this->_filename + compiler::COMPILER_FILE_EXTENSION_MAIN + "...");

	this->_logFileBuffer = std::make_shared<LogFileBuffer>(outpath + filename + compiler::COMPILER_FILE_EXTENSION_LOG);
	this->_tokenFileBuffer = std::make_shared<TokenFileBuffer>(outpath + filename + compiler::COMPILER_FILE_EXTENSION_TOKEN);
	
	this->_reservedWords = std::make_shared<ReservedWords>();
	this->_symbolTable = std::make_shared<SymbolTable>();
}

void Compiler::setupReservedWordsTable()
{
	this->addReservedWords();
}

void Compiler::addReservedWords()
{
	for (auto i : compiler::KEYWORDS) {
		this->_reservedWords->addReservedWord(i);
	}
}

void Compiler::setupSymbolTable()
{
	this->_symbolTable->linkTokenFileBuffer(this->_tokenFileBuffer);
}

void Compiler::printSymbolTable()
{
	this->_symbolTable->printTable();
}

void Compiler::setupLexicalAnalysis()
{
	this->_lexical = std::make_unique<Lexical>(this->_respath + this->_filename + compiler::COMPILER_FILE_EXTENSION_MAIN);
	this->_lexical->linkLogFileBuffer(this->_logFileBuffer);
	this->_lexical->linkTokenFileBuffer(this->_tokenFileBuffer);
	this->_lexical->linkReservedWords(this->_reservedWords);
	this->_lexical->linkSymbolTable(this->_symbolTable);
}

void Compiler::runLexicalAnalysis()
{
	this->_lexical->run();
}

void Compiler::setupSyntaxAnalysis()
{
	this->_syntax = std::make_unique<Syntax>();
	this->_syntax->linkLogFileBuffer(this->_logFileBuffer);
	this->_syntax->linkTokenFileBuffer(this->_tokenFileBuffer);
	this->_syntax->linkReservedWords(this->_reservedWords);
	this->_syntax->linkSymbolTable(this->_symbolTable);
}

void Compiler::runSyntaxAnalysis()
{
	this->_syntax->run();
}

void Compiler::run()
{
	// generate reserved words table
	this->setupReservedWordsTable();

	// symbol table
	this->setupSymbolTable();

	// run lexical analysis
	compiler::printConsoleInfo(compiler::COMPILER, "");
	compiler::printConsoleError(compiler::LEXICAL, "Starting Lexical Analysis...");
	this->setupLexicalAnalysis();
	this->runLexicalAnalysis();


	compiler::printConsoleInfo(compiler::SYMBOLTABLE, "Printing Symbol Table...");
	this->printSymbolTable();

	if (this->_lexical->getError()) {
		compiler::printConsoleError(compiler::LEXICAL, "An error has occured when reading the file.");
		compiler::printConsoleError(compiler::LEXICAL, "Look into the log file for more information.");
		compiler::printConsoleError(compiler::LEXICAL, _outpath + _filename + compiler::COMPILER_FILE_EXTENSION_LOG);
		return;
	}
	else {
		compiler::printConsoleInfo(compiler::LEXICAL, "Lexical Analysis Completed!");
	}


	// run syntax analysis
	this->setupSyntaxAnalysis();
	this->runSyntaxAnalysis();

	if (this->_syntax->getError()) {
		compiler::printConsoleError(compiler::SYNTAX, "An error has occured when parsing.");
		compiler::printConsoleError(compiler::SYNTAX, "Look into the log file for more information.");
		compiler::printConsoleError(compiler::SYNTAX, _outpath + _filename + compiler::COMPILER_FILE_EXTENSION_LOG);
		return;
	}
	else {
		compiler::printConsoleInfo(compiler::SYNTAX, "Sytanx Analysis Completed!");
	}

}
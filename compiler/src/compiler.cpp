#include "Compiler.h"

Compiler::Compiler(const std::string& filename, const std::string& outpath, const std::string& respath)
{
	this->_filename = filename;
	this->_outpath = outpath;
	this->_respath = respath;

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
	this->_reservedWords->addReservedWord("def");
	this->_reservedWords->addReservedWord("fed");
	this->_reservedWords->addReservedWord("int");
	this->_reservedWords->addReservedWord("double");
	this->_reservedWords->addReservedWord("if");
	this->_reservedWords->addReservedWord("then");
	this->_reservedWords->addReservedWord("else");
	this->_reservedWords->addReservedWord("fi");
	this->_reservedWords->addReservedWord("while");
	this->_reservedWords->addReservedWord("do");
	this->_reservedWords->addReservedWord("od");
	this->_reservedWords->addReservedWord("print");
	this->_reservedWords->addReservedWord("return");
	this->_reservedWords->addReservedWord("or");
	this->_reservedWords->addReservedWord("and");
	this->_reservedWords->addReservedWord("not");
}

void Compiler::setupSymbolTable()
{
	this->_symbolTable->linkTokenFileBuffer(this->_tokenFileBuffer);
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
	this->setupLexicalAnalysis();
	this->runLexicalAnalysis();

	if (this->_lexical->getError()) {
		std::cout << "LEXICAL ANALYSIS ERRORED" << std::endl;
		std::cout << "Please look into the log file for more information" << std::endl;
		std::cout << _outpath + _filename + compiler::COMPILER_FILE_EXTENSION_LOG << std::endl;
		return;
	}
	// run syntax analysis
	this->setupSyntaxAnalysis();
	this->runSyntaxAnalysis();

}
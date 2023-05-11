#include "Compiler.h"

Compiler::Compiler(const std::string& filename, const std::string& outpath, const std::string& respath)
{
	// TODO : filename will be actual file name
	// this->_fileNameMain = filename; 
	compiler::printConsoleInfo(compiler::COMPILER, "Starting compiler...");
	this->_fileNameRoot = filename; // TODO : WIll have to obtain the root of the file name
	this->_fileNameMain = filename + compiler::COMPILER_FILE_EXTENSION_MAIN;
	this->_outPath = outpath;
	this->_resPath = respath;
	compiler::printConsoleInfo(compiler::COMPILER, "Compiling " + this->_fileNameRoot + compiler::COMPILER_FILE_EXTENSION_MAIN + "...");


	this->_logFileBuffer = std::make_shared<LogFileBuffer>(outpath + filename + compiler::COMPILER_FILE_EXTENSION_LOG);

	this->_tokenFileBuffer = std::make_shared<TokenFileBuffer>(outpath + filename + compiler::COMPILER_FILE_EXTENSION_TOKEN);
	this->_symbolTableFileBuffer = std::make_shared<SymbolTableFileBuffer>(outpath + filename + compiler::COMPILER_FILE_EXTENSION_SYMBOL);

	this->_reservedWords = std::make_shared<ReservedWords>();
	this->_tokenList = std::make_shared<TokenList>();
}

void Compiler::run()
{
	// generate reserved words table
	compiler::printConsoleInfo(compiler::COMPILER, "Setting up reserved words table...");
	this->setupReservedWordsTable();
	compiler::printConsoleInfo(compiler::COMPILER, "Setting up token list...");
	this->setupTokenList();

	///////////////////////////////////////////////////////
	// LEXICAL
	compiler::printConsoleInfo(compiler::COMPILER, "");
	compiler::printConsoleInfo(compiler::COMPILER, "Starting lexical analysis...");
	this->setupLexicalAnalysis();
	this->runLexicalAnalysis();

	if (this->_lexical->getError()) {
		compiler::printConsoleError(compiler::LEXICAL, "An error has occured when reading the file.");
		compiler::printConsoleError(compiler::LEXICAL, "Look into the log file for more information.");
		compiler::printConsoleError(compiler::LEXICAL, this->_outPath + this->_fileNameRoot + compiler::COMPILER_FILE_EXTENSION_LOG);
		return;
	}

	compiler::printConsoleInfo(compiler::LEXICAL, "Lexical analysis completed!");
	compiler::printConsoleInfo(compiler::COMPILER, "Printing token list...");

	this->printTokenList();
}

bool Compiler::isFileValid()
{
	std::string filepath = this->_fileNameMain;
	if (filepath.length() < compiler::COMPILER_FILE_EXTENSION_MAIN_LEN + 1) {
		compiler::getConsoleError(compiler::COMPILER, "Invalid file string specified: length size");
		return false;
	}

	// check if directory or file

	if (filepath.substr(filepath.length() - compiler::COMPILER_FILE_EXTENSION_MAIN_LEN)
		!= compiler::COMPILER_FILE_EXTENSION_MAIN) {
		compiler::getConsoleError(compiler::COMPILER, "Invalid file string specified: file extension");
		return false;
	}

	return true;
}

void Compiler::addReservedWords()
{
	for (auto i : compiler::KEYWORDS) {
		this->_reservedWords->addReservedWord(i);
	}
}

void Compiler::setupReservedWordsTable()
{
	this->addReservedWords();
}

void Compiler::setupTokenList()
{
	this->_tokenList->linkTokenFileBuffer(this->_tokenFileBuffer);
}

void Compiler::printTokenList()
{
	this->_tokenList->print();
}

void Compiler::setupLexicalAnalysis()
{
	this->_lexical = std::make_unique<Lexical>(this->_resPath + this->_fileNameMain);
	this->_lexical->linkLogFileBuffer(this->_logFileBuffer);
	this->_lexical->linkReservedWords(this->_reservedWords);
	this->_lexical->linkTokenList(this->_tokenList);
}

void Compiler::runLexicalAnalysis()
{
	this->_lexical->run();
}

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
	this->_syntaxTree = std::make_shared<SyntaxTree>();
	this->_symbolTable = std::make_shared<SymbolTableManager>();
}

void Compiler::run()
{
	// generate reserved words table
	compiler::printConsoleInfo(compiler::COMPILER, "Setting up reserved words table...");
	this->setupReservedWordsTable();
	compiler::printConsoleInfo(compiler::COMPILER, "Setting up token list...");
	this->setupTokenList();
	compiler::printConsoleInfo(compiler::COMPILER, "Setting up syntax tree...");
	this->setupSyntaxTree();
	compiler::printConsoleInfo(compiler::COMPILER, "Setting up symbol table...");
	this->setupSymbolTable();

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

	///////////////////////////////////////////////////////
	// SYNTAX
	compiler::printConsoleInfo(compiler::COMPILER, "");
	compiler::printConsoleInfo(compiler::COMPILER, "Starting syntax analysis...");
	this->setupSyntaxAnalysis();
	this->runSyntaxAnalysis();

	if (this->_syntax->getError()) {
		compiler::printConsoleError(compiler::SYNTAX, "An error has occured when parsing.");
		compiler::printConsoleError(compiler::SYNTAX, "Look into the log file for more information.");
		compiler::printConsoleError(compiler::SYNTAX, this->_outPath + this->_fileNameRoot + compiler::COMPILER_FILE_EXTENSION_LOG);
		return;
	}

	compiler::printConsoleInfo(compiler::SYNTAX, "Syntax analysis completed!");
	//compiler::printConsoleInfo(compiler::COMPILER, "Printing syntax tree...");

	this->_syntaxTree->print();

	///////////////////////////////////////////////////////
	// SEMANTIC ANALYSIS
	compiler::printConsoleInfo(compiler::COMPILER, "");
	compiler::printConsoleInfo(compiler::COMPILER, "Starting semantic analysis...");
	this->setupSemanticAnalysis();
	this->runSemanticAnalysis();

	if (this->_semantic->getError()) {
		compiler::printConsoleError(compiler::SEMANTIC, "An error has occured when parsing.");
		compiler::printConsoleError(compiler::SEMANTIC, "Look into the log file for more information.");
		compiler::printConsoleError(compiler::SEMANTIC, this->_outPath + this->_fileNameRoot + compiler::COMPILER_FILE_EXTENSION_LOG);
		return;
	}

	compiler::printConsoleInfo(compiler::SEMANTIC, "Semantic analysis completed!");
	compiler::printConsoleInfo(compiler::COMPILER, "Printing symbol table...");

	this->_symbolTable->print();


	/////////////////////////////////////////////////////////
	//// IR GENERATION
	//this->setupICRGeneration();
	//this->runICRGeneration();


	//compiler::printConsoleInfo(compiler::COMPILER, "Program Complete!");
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

void Compiler::setupSyntaxTree()
{
	// TODO: 
}

void Compiler::printSyntaxTree()
{
	this->_symbolTable->print();
}

void Compiler::setupSymbolTable()
{
	this->_symbolTable->linkSymbolTableFileBuffer(this->_symbolTableFileBuffer);
}

void Compiler::printSymbolTable()
{
	this->_symbolTable->print();
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

void Compiler::setupSyntaxAnalysis()
{
	this->_syntax = std::make_unique<Syntax>();
	this->_syntax->linkLogFileBuffer(this->_logFileBuffer);
	this->_syntax->linkSyntaxTree(this->_syntaxTree);
	this->_syntax->linkTokenList(this->_tokenList);
}

void Compiler::runSyntaxAnalysis()
{
	this->_syntax->run();
}

void Compiler::setupSemanticAnalysis()
{
	this->_semantic = std::make_unique<Semantic>();
	this->_semantic->linkLogFileBuffer(this->_logFileBuffer);
	this->_semantic->linkTokenList(this->_tokenList);
	this->_semantic->linkSymbolTableManager(this->_symbolTable);
	this->_semantic->linkSyntaxTree(this->_syntaxTree);
}

void Compiler::runSemanticAnalysis()
{
	this->_semantic->run();
}

void Compiler::setupICRGeneration()
{
	// TODO: 
}

void Compiler::runICRGeneration()
{
	// TODO: 
}

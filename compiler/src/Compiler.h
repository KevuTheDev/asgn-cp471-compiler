#pragma once
#include <memory>

#include "globals.h"

#include "LogFileBuffer.h"
#include "TokenFileBuffer.h"
#include "SymbolTableFileBuffer.h"

#include "ReservedWords.h"
#include "TokenList.h"
#include "SyntaxTree.h"
#include "SymbolTableManager.h"

#include "Lexical.h"
#include "Syntax.h"
#include "Semantic.h"
#include "Intermediate.h"

class Compiler
{
public:
	Compiler(const std::string& filename, const std::string& outpath, const std::string& respath);

	void run();


private:
	bool isFileValid();

	// Reserved Words
	void addReservedWords();
	void setupReservedWordsTable();

	// Token List
	void setupTokenList();
	void printTokenList();

	// Syntax Tree
	void setupSyntaxTree();
	void printSyntaxTree();

	// Symbol Table
	void setupSymbolTable();
	void printSymbolTable();

	///////////////////////////////////

	// Lexical Analysis
	void setupLexicalAnalysis();
	void runLexicalAnalysis();

	// Syntax Analysis
	void setupSyntaxAnalysis();
	void runSyntaxAnalysis();

	// Semantic Analysis
	void setupSemanticAnalysis();
	void runSemanticAnalysis();

	// Intermediate Code Representation Generation
	void setupICRGeneration();
	void runICRGeneration();


private:
	std::string _fileNameMain;
	std::string _fileNameRoot;
	std::string _outPath;
	std::string _resPath;

	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<TokenFileBuffer> _tokenFileBuffer;
	std::shared_ptr<SymbolTableFileBuffer> _symbolTableFileBuffer;

	std::shared_ptr<ReservedWords> _reservedWords;
	std::shared_ptr<TokenList> _tokenList;
	std::shared_ptr<SyntaxTree> _syntaxTree;
	std::shared_ptr<SymbolTableManager> _symbolTable;

	std::unique_ptr<Lexical> _lexical;
	std::unique_ptr<Syntax> _syntax;
	std::unique_ptr<Semantic> _semantic;
	std::unique_ptr<Intermediate> _intermediate;

};


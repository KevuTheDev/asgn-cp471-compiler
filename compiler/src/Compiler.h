#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "globals.h"

#include "LogFileBuffer.h"
#include "TokenFileBuffer.h"

#include "ReservedWords.h"
#include "SymbolTable.h"

#include "Lexical.h"
#include "Syntax.h"

class Compiler
{
public:
	Compiler(const std::string& filename, const std::string& outpath, const std::string& respath);

	void run();

private:
	std::string _filename;
	std::string _outpath;
	std::string _respath;

	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<TokenFileBuffer> _tokenFileBuffer;

	std::shared_ptr<ReservedWords> _reservedWords;
	std::shared_ptr<SymbolTable> _symbolTable;

	std::unique_ptr<Lexical> _lexical;
	std::unique_ptr<Syntax> _syntax;


	// Reserved Words
	void setupReservedWordsTable();
	void addReservedWords();

	// Symbol Table
	void setupSymbolTable();
	void printSymbolTable();

	// Lexical
	void setupLexicalAnalysis();
	void runLexicalAnalysis();

	// Syntax
	void setupSyntaxAnalysis();
	void runSyntaxAnalysis();

};


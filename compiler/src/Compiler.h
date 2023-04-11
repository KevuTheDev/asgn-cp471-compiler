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

	LogFileBuffer* _logFileBuffer; 
	TokenFileBuffer* _tokenFileBuffer;

	ReservedWords* _reservedWords;
	SymbolTable* _symbolTable;

	Lexical* _lexical;
	Syntax* _syntax;


	// Reserved Words
	void setupReservedWordsTable();
	void addReservedWords();

	// Symbol Table
	void setupSymbolTable();

	// Lexical
	void setupLexicalAnalysis();
	void runLexicalAnalysis();

	// Syntax
	void setupSyntaxAnalysis();
	void runSyntaxAnalysis();

};


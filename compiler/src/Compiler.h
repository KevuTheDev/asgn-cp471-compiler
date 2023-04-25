#pragma once

#include <string>

#include "globals.h"

class Compiler
{
public:
	Compiler(const std::string& filename, const std::string& outpath, const std::string& respath);

	void run();

private:
	std::string _filename;
	std::string _outpath;
	std::string _respath;



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


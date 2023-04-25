#pragma once

#include <string>

#include "globals.h"

class Compiler
{
public:
	Compiler(const std::string& filename, const std::string& outpath, const std::string& respath);

	void run();


private:
	// Reserved Words
	void setupReservedWordsTable();
	void addReservedWords();

	// Symbol Table
	void setupSymbolTable();
	void printSymbolTable();

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
	std::string _filename;
	std::string _outpath;
	std::string _respath;
};


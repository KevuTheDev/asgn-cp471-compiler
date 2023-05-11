#pragma once
#include <memory>

#include "globals.h"

#include "LogFileBuffer.h"
#include "TokenFileBuffer.h"
#include "SymbolTableFileBuffer.h"

#include "ReservedWords.h"
#include "TokenList.h"

#include "Lexical.h"

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
	///////////////////////////////////

	// Lexical Analysis
	void setupLexicalAnalysis();
	void runLexicalAnalysis();


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

	std::unique_ptr<Lexical> _lexical;
};


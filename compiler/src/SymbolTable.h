#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <format>

#include "TokenFileBuffer.h"

struct SymbolRow {
	compiler::TOKEN token;
	std::string lexeme;
	int lineNumber;
	int charNumber;
};


class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	void linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer);

	bool append(compiler::TOKEN token, std::string lexeme, int lineNumber, int charNumber);
	void printTable();

	int length();
	compiler::TOKEN getTokenAtIndex(int index);
	SymbolRow getSymbolRowAtIndex(int index);

private:
	// token, lexeme, line number, char number limits
	int _printTokenLimit;
	int _printLexemeLimit;
	int _printLineNumLimit;
	int _printCharNumLimit;

	std::vector <SymbolRow> _table;
	std::shared_ptr<TokenFileBuffer> _tokenFileBuffer;

};


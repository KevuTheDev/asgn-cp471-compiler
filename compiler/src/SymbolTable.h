#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "TokenFileBuffer.h"

struct SymbolRow {
	std::string token;
	std::string lexeme;
	int lineNumber;
};

struct RowSize {
	int limit;
	int length;
};

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	void linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer);

	bool append(std::string token, std::string lexeme, int lineNumber);
	void printTable();

	int length();
	std::string getTokenAtIndex(int index);

private:
	// token, lexeme, line number
	RowSize _tableToken;
	RowSize _tableLexeme;
	RowSize _tableLineNum;

	std::vector <SymbolRow> _table;

	std::shared_ptr<TokenFileBuffer> _tokenFileBuffer;
};


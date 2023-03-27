#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "FileBuffer.h"
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

	bool append(std::string token, std::string lexeme, int lineNumber);
	void printTable();

private:
	// token, lexeme, line number
	RowSize _tableToken;
	RowSize _tableLexeme;
	RowSize _tableLineNum;

	std::vector < SymbolRow> _table;
};


#pragma once
#include <string>
#include <tuple>
#include <vector>

struct SymbolRow {
	std::string token;
	std::string lexeme;
	int lineNumber;
};

class SymbolTable
{
public:
	SymbolTable();

	bool append(std::string token, std::string lexeme, int lineNumber);

private:
	// token, lexeme, line number
	std::vector < SymbolRow> _table;
};


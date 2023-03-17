#pragma once
#include <string>
#include <tuple>
#include <vector>

class SymbolTable
{
public:
	SymbolTable();

	bool append(std::string token, std::string lexeme, int lineNumber);

private:
	// token, lexeme, line number
	std::vector < std::tuple<std::string, std::string, int>> _table;
};


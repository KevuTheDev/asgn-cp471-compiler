#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <format>

#include "SymbolTableFileBuffer.h"



class SymbolTable
{
private:
	struct SymbolRow {
		compiler::TOKEN token;
		std::string lexeme;
		int lineNumber;
		int charNumber;
	};

public:
	SymbolTable();
	~SymbolTable();

	void linkSymbolTableFileBuffer(std::shared_ptr<SymbolTableFileBuffer> buffer);

	void print();


private:
	std::shared_ptr<SymbolTableFileBuffer> _symbolTableFileBuffer;
	std::vector <SymbolRow> _table;


	// token, lexeme, line number, char number limits
	int _printTokenLimit;
	int _printLexemeLimit;
	int _printLineNumLimit;
	int _printCharNumLimit;


};


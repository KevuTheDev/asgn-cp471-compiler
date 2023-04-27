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
public:
	struct SymbolTableRow {
		uint32_t lineNum;
		uint32_t charPos;
		compiler::TOKEN token; // so questionable, do i need it?
		std::string lexeme;
		std::string scope;
		std::string type;
		std::string category;
		std::shared_ptr<SymbolTable> childrenScope = nullptr;
		std::vector<std::string> params;

		void addParamType(std::string type) {
			this->params.push_back(type);
		};
	};

	SymbolTable(std::string tablename);
	~SymbolTable();

	void append(std::shared_ptr<SymbolTableRow> row);

	std::vector <std::shared_ptr<SymbolTable::SymbolTableRow>> getSymbolTable() const;

	uint32_t getSize();

public:
	// token, lexeme, line number, char number limits
	static uint32_t _printLineNumLimit;
	static uint32_t _printCharPosLimit;
	static uint32_t _printTokenLimit;
	static uint32_t _printLexemeLimit;
	static uint32_t _printScopeLimit;
	static uint32_t _printTypeLimit;
	static uint32_t _printCategoryLimit;
	static uint32_t _printParamsLimit;

private:

private:
	std::string _tableName;

	std::vector <std::shared_ptr<SymbolTableRow>> _symbolTable;

};
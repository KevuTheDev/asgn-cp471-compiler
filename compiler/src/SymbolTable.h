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
	SymbolTable(std::string tablename);
	~SymbolTable();

	void linkSymbolTableFileBuffer(std::shared_ptr<SymbolTableFileBuffer> buffer);

	void print();
public:
	// token, lexeme, line number, char number limits
	static uint32_t _printLineNumLimit;
	static uint32_t _printCharPosLimit;
	static uint32_t _printTokenLimit;
	static uint32_t _printLexemeLimit;
	static uint32_t _printScopeLimit;
	static uint32_t _printTypeLimit;
	static uint32_t _printCategoryLimit;

private:
	struct SymbolTableRow {
		uint32_t lineNum;
		uint32_t charPos;
		compiler::TOKEN token;
		std::string lexeme;
		std::string scope;
		std::string type;
		std::string category;
		std::shared_ptr<SymbolTable> parentScope;
		std::shared_ptr<SymbolTable> childrenScope;
	};

private:
	std::shared_ptr<SymbolTableFileBuffer> _symbolTableFileBuffer;
	std::vector <std::shared_ptr<SymbolTableRow>> _symbolTable;

	std::string _tableName;




};
#pragma once
#include <format>
#include <memory>
#include <stack>

#include "SymbolTable.h"

class SymbolTableManager
{

public:
	SymbolTableManager();
	~SymbolTableManager();

	void linkSymbolTableFileBuffer(std::shared_ptr<SymbolTableFileBuffer> buffer);

	void enterScope(std::string scope);
	void exitScope();
	
	void appendRow(std::shared_ptr<SymbolTable::SymbolTableRow>); // TODO: params, and add nodes 

	bool isIdentifierInCurrentScope(std::string id);


	void print();

private:
	void calculatePrintLimits();

	void print_aux(const std::string& padding, uint16_t rightPadding, std::vector<std::shared_ptr<SymbolTable::SymbolTableRow>> table);

private:
	std::shared_ptr<SymbolTableFileBuffer> _symbolTableFileBuffer;

	std::stack<std::shared_ptr<SymbolTable>> _tableStack;

	std::shared_ptr<SymbolTable> _head;
	std::shared_ptr<SymbolTable::SymbolTableRow> _currentRow;


	const std::string symbolTableHeader[6] = { "LINE", "CHAR", "LEXEME", 
		"SCOPE", "TYPE", "CATEGORY"};
	const uint16_t symbolTableHeaderSize[6] = { 4, 4, 6, 5, 4, 8 };

};


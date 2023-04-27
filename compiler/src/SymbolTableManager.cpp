#include "SymbolTableManager.h"

SymbolTableManager::SymbolTableManager()
{
	this->_head = nullptr;
	this->_currentRow = nullptr;
}

SymbolTableManager::~SymbolTableManager()
{
}

void SymbolTableManager::linkSymbolTableFileBuffer(std::shared_ptr<SymbolTableFileBuffer> buffer)
{
	this->_symbolTableFileBuffer = buffer;
}

void SymbolTableManager::enterScope(std::string scope)
{

	// if the head is not initialized
	if (this->_head == nullptr) {
		this->_head = std::make_shared<SymbolTable>(scope);
		this->_tableStack.push(this->_head);
		return;
	}
	else {
		auto newTable = std::make_shared<SymbolTable>(scope);
		this->_currentRow->childrenScope = newTable; // this->_currentRow has to exist
		this->_tableStack.push(newTable);
	}

}

void SymbolTableManager::exitScope()
{
	this->_tableStack.pop();
}

void SymbolTableManager::appendRow(std::shared_ptr<SymbolTable::SymbolTableRow> row)
{
	this->_currentRow = row;
	this->_tableStack.top()->append(row);

	uint32_t lineLength = (uint32_t) std::to_string(row->lineNum).length();
	uint32_t charLength = (uint32_t) std::to_string(row->charPos).length();
	uint32_t lexeme = (uint32_t) row->lexeme.length();
	uint32_t scope = (uint32_t) row->scope.length();
	uint32_t type = (uint32_t) row->type.length();
	uint32_t category = (uint32_t) row->category.length();

	SymbolTable::_printLineNumLimit = (lineLength > SymbolTable::_printLineNumLimit) ? lineLength : SymbolTable::_printLineNumLimit;
	SymbolTable::_printCharPosLimit = (charLength > SymbolTable::_printCharPosLimit) ? charLength : SymbolTable::_printCharPosLimit;
	SymbolTable::_printLexemeLimit = (lexeme > SymbolTable::_printLexemeLimit) ? lexeme : SymbolTable::_printLexemeLimit;
	SymbolTable::_printScopeLimit = (scope > SymbolTable::_printScopeLimit) ? scope : SymbolTable::_printScopeLimit;
	SymbolTable::_printTypeLimit = (type > SymbolTable::_printTypeLimit) ? type : SymbolTable::_printTypeLimit;
	SymbolTable::_printCategoryLimit = (category > SymbolTable::_printCategoryLimit) ? category : SymbolTable::_printCategoryLimit;
}

bool SymbolTableManager::isIdentifierInCurrentScope(std::string id)
{
	// i have to backtrack to see if the symbol exists

	// there are different ways of writing this
	
	// 1. if the ID is in the symbol table at all scope, do not add them (only accept unique ID values)
	// 2. variable ID and function ID can exist in the scope it is within
	// 3. variables can be redeclared, changing the definition of variables.

	// Any of these options work, but I have to mention it

	std::shared_ptr<SymbolTable> table = nullptr;

	table = this->_head;

	std::shared_ptr<SymbolTable::SymbolTableRow> row = nullptr;

	while (table != nullptr) {
		row = nullptr;

		auto iter = table->getSymbolTable();
		for (auto i : iter) {
			if (i->lexeme == id) {
				return true;
			}
			row = i;
		}

		table = (row == nullptr)? nullptr : row->childrenScope;
	}

	//std::cout << "OH NO" << std::endl;
	return false;
}

void SymbolTableManager::print()
{
	uint16_t attributeNumber = 6; // Number of attribute placment
	uint16_t attributePadding = 1; // size of padding between word and vertical line
	uint16_t tableVerticalLines = attributeNumber + 1; // number of vertical lines 
	uint16_t rightPadding = attributePadding + 6;

	uint16_t tableHeadSize = attributeNumber * (attributePadding + rightPadding) + tableVerticalLines;

	tableHeadSize += SymbolTable::_printLineNumLimit + SymbolTable::_printCharPosLimit + SymbolTable::_printLexemeLimit
		+ SymbolTable::_printScopeLimit + SymbolTable::_printTypeLimit + SymbolTable::_printCategoryLimit;





	std::string tableHead = std::string(tableHeadSize, '=');
	std::string padding = std::string(attributePadding, ' ');

	// print table head
	this->_symbolTableFileBuffer->writeToFile(tableHead);
	std::cout << tableHead << std::endl;

	std::string header = std::format("|{}{}{}|{}{}{}|{}{}{}|{}{}{}|{}{}{}|{}{}{}|",
		padding, symbolTableHeader[0], std::string(SymbolTable::_printLineNumLimit - symbolTableHeaderSize[0]
			+ rightPadding, ' '),
		padding, symbolTableHeader[1], std::string(SymbolTable::_printCharPosLimit - symbolTableHeaderSize[1]
			+ rightPadding, ' '),
		padding, symbolTableHeader[2], std::string(SymbolTable::_printLexemeLimit - symbolTableHeaderSize[2]
			+ rightPadding, ' '),
		padding, symbolTableHeader[3], std::string(SymbolTable::_printScopeLimit - symbolTableHeaderSize[3]
			+ rightPadding, ' '),
		padding, symbolTableHeader[4], std::string(SymbolTable::_printTypeLimit - symbolTableHeaderSize[4]
			+ rightPadding, ' '),
		padding, symbolTableHeader[5], std::string(SymbolTable::_printCategoryLimit - symbolTableHeaderSize[5]
			+ rightPadding, ' ')
	);

	this->_symbolTableFileBuffer->writeToFile(header);
	std::cout << header << std::endl;

	this->_symbolTableFileBuffer->writeToFile(tableHead);
	std::cout << tableHead << std::endl;

	print_aux(padding, rightPadding, this->_head->getSymbolTable());

	this->_symbolTableFileBuffer->writeToFile(tableHead);
	std::cout << tableHead << std::endl;
}

void SymbolTableManager::calculatePrintLimits()
{
	SymbolTable::_printLineNumLimit = (symbolTableHeaderSize[0] > SymbolTable::_printLineNumLimit) ? symbolTableHeaderSize[0] : SymbolTable::_printLineNumLimit;
	SymbolTable::_printCharPosLimit = (symbolTableHeaderSize[1] > SymbolTable::_printCharPosLimit) ? symbolTableHeaderSize[1] : SymbolTable::_printCharPosLimit;
	SymbolTable::_printLexemeLimit = (symbolTableHeaderSize[2] > SymbolTable::_printLexemeLimit) ? symbolTableHeaderSize[2] : SymbolTable::_printLexemeLimit;
	SymbolTable::_printScopeLimit = (symbolTableHeaderSize[3] > SymbolTable::_printScopeLimit) ? symbolTableHeaderSize[3] : SymbolTable::_printScopeLimit;
	SymbolTable::_printTypeLimit = (symbolTableHeaderSize[4] > SymbolTable::_printTypeLimit) ? symbolTableHeaderSize[4] : SymbolTable::_printTypeLimit;
	SymbolTable::_printCategoryLimit = (symbolTableHeaderSize[5] > SymbolTable::_printCategoryLimit) ? symbolTableHeaderSize[5] : SymbolTable::_printCategoryLimit;
}

void SymbolTableManager::print_aux(const std::string& padding, uint16_t rightPadding, std::vector<std::shared_ptr<SymbolTable::SymbolTableRow>> table)
{

	std::string sLineNum;
	std::string sCharPos;
	for (auto i : table) {

		sLineNum = std::to_string(i->lineNum);
		sCharPos = std::to_string(i->charPos);


		std::string rows = std::format("|{}{}{}|{}{}{}|{}{}{}|{}{}{}|{}{}{}|{}{}{}|",
			padding, sLineNum, std::string(SymbolTable::_printLineNumLimit - sLineNum.length()
				+ rightPadding, ' '),
			padding, sCharPos, std::string(SymbolTable::_printCharPosLimit - sCharPos.length()
				+ rightPadding, ' '),
			padding, i->lexeme, std::string(SymbolTable::_printLexemeLimit - i->lexeme.length()
				+ rightPadding, ' '),
			padding, i->scope, std::string(SymbolTable::_printScopeLimit - i->scope.length()
				+ rightPadding, ' '),
			padding, i->type, std::string(SymbolTable::_printTypeLimit - i->type.length()
				+ rightPadding, ' '),
			padding, i->category, std::string(SymbolTable::_printCategoryLimit - i->category.length()
				+ rightPadding, ' ')
		);
		this->_symbolTableFileBuffer->writeToFile(rows);
		std::cout << rows << std::endl;

		if (i->childrenScope != nullptr) {
			print_aux(padding, rightPadding, i->childrenScope->getSymbolTable());
		}
	}
}

#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "globals.h"
#include "ReservedWords.h"
#include "SymbolTable.h"
#include "LogFileBuffer.h"
#include "TokenFileBuffer.h"


class Lexical
{
public:
	Lexical(const std::string &filename);

	void linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer);
	void linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer);
	void linkReservedWords(std::shared_ptr<ReservedWords> table);
	void linkSymbolTable(std::shared_ptr<SymbolTable> table);

	void run();

private:
	std::ifstream _is;
	std::string _fileName;
		
	char _peek;
	uint32_t _lineNumber;
	uint32_t _charNumber;

	char* _doubleBuffer1;
	char* _doubleBuffer2;
	uint16_t _doubleBufferCounter1;
	uint16_t _doubleBufferCounter2;
	bool _doubleBufferSwitch; // buffer1 - true, buffer2 - false

	///
	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<TokenFileBuffer> _tokenFileBuffer;

	std::shared_ptr<ReservedWords> _reservedWords;
	std::shared_ptr<SymbolTable> _symbolTable;


	

	// Utilities
	//		-> Lexical Related
	bool checkExtension(const std::string& filepath);
	void readFileToBuffer(std::ifstream& is, char* buf);

	//		-> Token File Buffer Related
	void sanitizeFileName(const std::string& filepath);

	//		-> Log File Buffer Related
	void appendToLogFileBuffer(int linenumber, int rownumber, const std::string& errorchar);

	//		-> Symbol Table Related
	void appendToSymbolTable(std::string token, std::string lexeme, int lineNumber);



	// Lexical Analysis
	bool getNextToken(); // bool: true- continue, false- eof/exit
	void readNextChar();
	bool readNextChar(char c); // this does the peaking for specific grammar


};


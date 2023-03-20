#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include "globals.h"
#include "SymbolTable.h"
#include "TokenFileBuffer.h"


class Lexical
{
public:
	Lexical(const std::string &filename);

private:
	std::ifstream _is;
	std::string _fileName;

	TokenFileBuffer _tokenFileBuffer;
	SymbolTable _symbolTable;



	char _peek;
	uint32_t _lineNumber;

	char* _doubleBuffer1;
	char* _doubleBuffer2;
	uint16_t _doubleBufferCounter1;
	uint16_t _doubleBufferCounter2;
	bool _doubleBufferSwitch; // buffer1 - true, buffer2 - false


	

	// Utilities
	//		-> Lexical Related
	bool checkExtension(const std::string& filepath);
	void readFileToBuffer(std::ifstream& is, char* buf);

	//		-> Token File Buffer Related
	void sanitizeFileName(const std::string& filepath);
	void appendToTokenFileBuffer(const char* token);
	void appendToTokenFileBuffer(const std::string& token);
	void appendToTokenFileBuffer(int token);
	void appendToTokenFileBuffer(double token);

	//		-> Symbol Table Related
	void appendToSymbolTable(std::string token, std::string lexeme, int lineNumber);



	// Lexical Analysis
	bool getNextToken(); // bool: true- continue, false- eof/exit
	void readNextChar();
	bool readNextChar(char c); // this does the peaking for specific grammar


};


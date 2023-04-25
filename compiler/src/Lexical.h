#pragma once
#include <memory>
#include <thread>

#include "LogFileBuffer.h"
#include "ReservedWords.h"
#include "TokenList.h"

class Lexical
{
public:
	Lexical(const std::string &filename);
	~Lexical();

	void linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer);
	void linkReservedWords(std::shared_ptr<ReservedWords> table);
	void linkTokenList(std::shared_ptr<TokenList> list);

	void run();
	bool getError();

private:
	bool checkExtension(const std::string& filepath);
	
	// Double Buffer Threading function
	void readFileToBuffer(std::ifstream& is, char* buf);
	
	// Error Handling
	void reportError(const std::string& errorchar);

	void appendToTokenList(compiler::TOKEN token, std::string lexeme);

	// Lexical Analysis
	void resetPeek();
	bool getNextToken(); // bool: true- continue, false- eof/exit
	void readNextChar();
	bool readNextChar(char c); // this does the peaking for specific grammar


private:
	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<ReservedWords> _reservedWords;
	std::shared_ptr<TokenList> _tokenList;

	std::ifstream _is;
	std::string _fileName;
	bool _error;


	char _peek;
	uint32_t _lineNumber;
	uint32_t _charPosition;
	uint32_t _prevCharPosition;
	std::string _currentLine;


	char* _buffer1;
	char* _buffer2;
	uint16_t _bufferCounter1;
	uint16_t _bufferCounter2;
	bool _bufferSwitch; // buffer1 - true, buffer2 - false
};


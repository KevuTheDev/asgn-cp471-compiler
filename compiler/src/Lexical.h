#pragma once
#include <memory>

#include "LogFileBuffer.h"
#include "ReservedWords.h"
#include "TokenList.h"

class Lexical
{
public:
	Lexical();
	~Lexical();

	void linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer);
	void linkReservedWords(std::shared_ptr<ReservedWords> table);
	void linkTokenList(std::shared_ptr<TokenList> list);

	void run();

	bool getError();

private:

private:
	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<ReservedWords> _reservedWords;
	std::shared_ptr<TokenList> _tokenList;

	bool _error;

};


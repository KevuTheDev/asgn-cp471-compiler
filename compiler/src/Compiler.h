#pragma once

#include <iostream>
#include <string>

#include "globals.h"

#include "LogFileBuffer.h"
#include "TokenFileBuffer.h"

#include "ReservedWords.h"
#include "SymbolTable.h"

#include "Lexical.h"
#include "Syntax.h"

class Compiler
{
public:
	Compiler(const std::string& filename);

	void run();

	~Compiler();

private:
	std::string _filename;

	LogFileBuffer *_logfilebuffer;
	TokenFileBuffer *_tokenfilebuffer;

	ReservedWords *_reservedwords;
	SymbolTable *_symboltable;
	

	Lexical *_lexical;

	Syntax *_syntax;

};


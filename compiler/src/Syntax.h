#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "globals.h"
#include "SymbolTable.h"
#include "LogFileBuffer.h"
#include "TokenFileBuffer.h"


class Syntax
{
public:
	Syntax();
	void start();

private:
	std::string _peek;
	int _position;
	int _limit;


	void matchToken(const std::string& token);
	void getNextToken();

	

	// states

	void E();
	void E1();

	void T();
	void T1();

	void F();

};


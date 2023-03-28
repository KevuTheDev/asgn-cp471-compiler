#pragma once
#include <memory>


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



	void E();
	void E1();

	void T();
	void T1();

	void F();
};


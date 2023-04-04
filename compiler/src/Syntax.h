#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "globals.h"

#include "ReservedWords.h"
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

	std::string getPeek();

	

	// states

	bool NT_PROGRAM();
	bool NT_FDECLS();
	bool NT_FDEC();
	bool NT_PARAMS();
	bool NT_PARAMS_EXT();
	bool NT_FNAME();
	bool NT_DECLARATIONS();
	bool NT_DECL();
	bool NT_TYPE();
	bool NT_VARLIST();
	bool NT_VARLIST_EXT();
	bool NT_STATEMENT();
	bool NT_STATEMENT_EXT();
	bool NT_STATEMENTSEQ();
	bool NT_STATEMENTSEQ_EXT();
	bool NT_EXPR();
	bool NT_EXPR_EXT();
	bool NT_TERM();
	bool NT_TERM_EXT();
	bool NT_FACTOR();
	bool NT_FACTOR_EXT();
	bool NT_EXPRSEQ();
	bool NT_EXPRSEQ_EXT();
	bool NT_BEXPR();
	bool NT_BEXPR_EXT();
	bool NT_BTERM();
	bool NT_BTERM_EXT();
	bool NT_BFACTOR();
	bool NT_BFACTOR_EXT();
	bool NT_COMP();
	bool NT_VAR();
	bool NT_VAR_EXT();
	bool NT_NUMBER();


	bool E();
	bool E1();

	bool T();
	bool T1();

	bool F();

};


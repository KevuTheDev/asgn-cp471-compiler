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
	~Syntax();

	void linkLogFileBuffer(std::shared_ptr<LogFileBuffer> buffer);
	void linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer);
	void linkReservedWords(std::shared_ptr<ReservedWords> table);
	void linkSymbolTable(std::shared_ptr<SymbolTable> table);

	void run();

private:
	std::string _peek;
	int _position;
	int _limit;


	void matchToken(const std::string& token);
	bool matchTokenNew(const std::string& token);
	void getNextToken();

	std::string getPeek();

	std::string readString = "";


	std::shared_ptr<LogFileBuffer> _logFileBuffer;
	std::shared_ptr<TokenFileBuffer> _tokenFileBuffer;

	std::shared_ptr<ReservedWords> _reservedWords;
	std::shared_ptr<SymbolTable> _symbolTable;


	void start();

	// states
	bool PROGRAM();
	bool FDECLS();
	bool FDECLS_EXT();
	bool FDEC();
	bool PARAMS();
	bool PARAMS_EXT();
	bool FNAME();
	bool DECLARATIONS();
	bool DECLARATIONS_EXT();
	bool DECL();
	bool TYPE();
	bool VARLIST();
	bool VARLIST_EXT();
	bool STATEMESEQ();
	bool STATEMESEQ_EXT();
	bool STATEMENT();
	bool STATEMEEXT();
	bool EXPR();
	bool EXPR_EXT();
	bool TERM();
	bool TERM_EXT();
	bool FACTOR();
	bool FACTOR_EXT();
	bool EXPRSEQ();
	bool EXPRSEQ_EXT();
	bool BEXPR();
	bool BEXPR_EXT();
	bool BTERM();
	bool BTERM_EXT();
	bool BFACTOR();
	bool BFACTOR_EXT();
	bool COMP();
	bool VAR();
	bool VAR_EXT();
	bool NUMBER();


	// Simple Grammar
	bool E();
	bool E1();
	bool T();
	bool T1();
	bool F();

};


/* Syntax Simple Grammar v1
	void E();
	void E1();

	void T();
	void T1();

	void F();
*/
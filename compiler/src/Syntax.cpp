#include "Syntax.h"

extern std::unique_ptr<SymbolTable> SYMBOL_TABLE;
extern std::unique_ptr<LogFileBuffer> LOG_FILE_BUFFER;
extern std::unique_ptr<TokenFileBuffer> TOKEN_FILE_BUFFER;

Syntax::Syntax()
{
}

void Syntax::start()
{
	E();
}

void Syntax::E()
{

}

void Syntax::E1()
{

}

void Syntax::T()
{

}

void Syntax::T1()
{

}

void Syntax::F()
{

}

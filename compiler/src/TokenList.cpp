#include "TokenList.h"

TokenList::TokenList()
{
}

TokenList::~TokenList()
{
}

void TokenList::linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer)
{
	this->_tokenFileBuffer = buffer;
}

void TokenList::print()
{
}

#pragma once
#include <memory>

#include "TokenFileBuffer.h"

class TokenList
{
public:
	TokenList();
	~TokenList();

	void linkTokenFileBuffer(std::shared_ptr<TokenFileBuffer> buffer);

	void print();

private:
	struct TokenNode {

	};

private:
	std::shared_ptr<TokenFileBuffer> _tokenFileBuffer;
};


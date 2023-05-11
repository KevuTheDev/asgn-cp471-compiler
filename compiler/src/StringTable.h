#pragma once
#include <unordered_map>
#include <string>
#include "globals.h"

class StringTable
{
public:
	StringTable();
	~StringTable();



private:
	std::unordered_map<std::string, compiler::TOKEN> _table;
};


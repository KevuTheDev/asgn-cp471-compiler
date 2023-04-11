#pragma once
#include <iostream>
#include <string>
#include <vector>

class ReservedWords
{
public:
	ReservedWords();
	~ReservedWords();

	void addReservedWord(std::string word);

	bool findReservedWord(const std::string& word);



private:
	std::vector <std::string> _table;
};


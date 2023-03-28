#pragma once
#include <iostream>
#include <string>
#include <vector>

class ReservedWords
{
public:
	ReservedWords();
	void addReservedWord(std::string word);

	void findReservedWord(const std::string& word);



private:
	std::vector <std::string> _table;
};


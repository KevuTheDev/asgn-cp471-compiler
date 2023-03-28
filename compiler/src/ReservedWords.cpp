#include "ReservedWords.h"

ReservedWords::ReservedWords()
{
	this->_table = std::vector<std::string>();
}

void ReservedWords::addReservedWord(std::string word)
{
	this->_table.push_back(word);
}

bool ReservedWords::findReservedWord(const std::string& word)
{
    auto it = std::find(this->_table.begin(), this->_table.end(), word);

    if (it != this->_table.end()) {
        // std::cout << "Found " << word << " at position " << std::distance(this->_table.begin(), it) << std::endl;
        return true;
    }
    else {
        // std::cout << word << " not found." << std::endl;
        return false;
    }
}

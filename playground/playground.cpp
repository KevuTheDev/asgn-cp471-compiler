#include <iostream>
#include <unordered_map>
#include <string>

#include <typeinfo>

#include <vector>

#include "B.h"

int main()
{

	std::vector<A*> v;

	v.push_back(new A());
	v.push_back(new B());

	std::cout << std::boolalpha << std::is_same_v<decltype(v[1]), decltype(v[0])> << std::endl;



	return 0;

}
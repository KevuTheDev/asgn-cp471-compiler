#include <iostream>
#include <unordered_map>
#include <string>

int main()
{
	auto mapp = std::unordered_map<std::string, int>();

	mapp["hello"] = 123;
	mapp["world"] = 456;
	mapp["ice"] = 1234;
	mapp["hello3"] = 123;
	mapp["worl4d"] = 456;
	mapp["ic2e"] = 1234;

	mapp["hel3lo"] = 123;
	mapp["wor2ld"] = 456;
	mapp["ic2e"] = 1234;
	mapp["he5llo3"] = 123;
	mapp["wor2l4d"] = 456;
	mapp["ic42e"] = 1234;

	int* mappping = &(mapp["hello"]);
	auto nn = mapp.find("hello");

	return 0;

}
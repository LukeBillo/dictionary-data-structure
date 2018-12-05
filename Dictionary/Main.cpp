#include "dictionary.h"
#include <string>
#include <iostream>

int main()
{
	auto dictionary = new Containers::Dictionary<std::string, int>();
	dictionary->insert("Test", 1);
	const auto testInsert = dictionary->lookup("Test");

	std::cout << *testInsert << std::endl;


    return 0;
}


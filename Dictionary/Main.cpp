#include "dictionary.h"
#include <string>
#include <regex>
#include <iostream>

bool doesKeyContainRemove(Containers::Dictionary<std::string, int>::Key key)
{
	const std::regex containsTestRegex("test");
	bool isMatch = std::regex_search(key, containsTestRegex);
	return isMatch;
}

int main()
{
	auto dictionary = new Containers::Dictionary<std::string, int>();
	
	const std::string keyBaseName = "test-";
	for (int i = 0; i < 1000; ++i)
	{
		const std::string keyName = keyBaseName + std::to_string(i);
		dictionary->insert(keyName, i);
	}

	dictionary->removeIf(doesKeyContainRemove);
	std::cout << *dictionary << std::endl;

	delete dictionary;

    return 0;
}

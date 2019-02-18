#include "dictionary.h"
#include <string>
#include <regex>
#include <iostream>
#include <cassert>

bool doesKeyContainRemove(const Containers::Dictionary<std::string, int>::Key key)
{
	const std::regex containsTestRegex("test");
	const bool isMatch = std::regex_search(key, containsTestRegex);
	return isMatch;
}

int main()
{
	const auto dictionary = new Containers::Dictionary<std::string, int>();
	dictionary->insert("hello", 1);

	const auto copiedDictionary = new Containers::Dictionary<std::string, int>(*dictionary);

	auto result = *dictionary == *copiedDictionary;

    return 0;
}

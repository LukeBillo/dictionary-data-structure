#include "pch.h"
#include "../Dictionary/dictionary.h"
#include <regex>

TEST(DictionaryInsertAndRemoveTests, WhenInsertingAndRemovingOneKeyItemPair)
{
	auto dictionary = new Containers::Dictionary<std::string, int>();
	const auto key = "test-remove";
	const auto item = 0;

	dictionary->insert(key, item);
	dictionary->remove(key);

	auto lookupRemovedItem = dictionary->lookup(key);

	EXPECT_EQ(lookupRemovedItem, nullptr);
}

bool doesKeyContainRemove(Containers::Dictionary<std::string, int>::Key key)
{
	const std::regex containsTestRegex("test");
	bool isMatch = std::regex_search(key, containsTestRegex);
	return isMatch;
}

TEST(DictionaryInsertAndRemoveTests, WhenInsertingAndRemovingIfKeyContainsTest)
{
	auto dictionary = new Containers::Dictionary<std::string, int>();

	dictionary->insert("isNotRemoved-1", 100);

	const std::string keyBaseName = "test-";
	for (int i = 0; i < 10; ++i)
	{
		const std::string keyName = keyBaseName + std::to_string(i);
		dictionary->insert(keyName, i);
	}

	dictionary->insert("isNotRemoved-2", 101);
	dictionary->removeIf(doesKeyContainRemove);

	auto shouldBeRemoved = dictionary->lookup("test-1");
	EXPECT_EQ(shouldBeRemoved, nullptr);

	auto shouldNotHaveBeenRemoved = dictionary->lookup("isNotRemoved-1");
	EXPECT_EQ(*shouldNotHaveBeenRemoved, 100);

	auto alsoShouldNotBeRemoved = dictionary->lookup("isNotRemoved-2");
	EXPECT_EQ(*alsoShouldNotBeRemoved, 101);
}
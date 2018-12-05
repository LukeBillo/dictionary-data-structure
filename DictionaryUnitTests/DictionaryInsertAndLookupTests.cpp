#include "pch.h"
#include "../Dictionary/dictionary.h"

TEST(DictionaryInsertAndLookupTests, WhenReadingANonExistentKeyItemPair)
{
	auto dictionary = new Containers::Dictionary<std::string, int>();
	auto* nonExistentItem = dictionary->lookup("DoesntExist");

	EXPECT_EQ(nonExistentItem, nullptr);
}

TEST(DictionaryInsertAndLookupTests, WhenInsertingAndReadingOneKeyItemPair)
{
	auto dictionary = new Containers::Dictionary<std::string, int>();
	dictionary->insert("Test", 0);

	auto* insertedItem = dictionary->lookup("Test");

	EXPECT_EQ(*insertedItem, 0);
}

TEST(DictionaryInsertAndLookupTests, WhenInsertingAndReadingMultipleKeyItemPairs)
{
	auto dictionary = new Containers::Dictionary<std::string, int>();

	const std::string keyBaseName = "test-";
	for (int i = 0; i < 10; ++i)
	{
		const std::string keyName = keyBaseName + std::to_string(i);
		dictionary->insert(keyName, i);

		auto* lookedUpItem = dictionary->lookup(keyName);
		EXPECT_EQ(*lookedUpItem, i);
	}

	auto* testLookupAfterAllInserts = dictionary->lookup("test-2");
	EXPECT_EQ(*testLookupAfterAllInserts, 2);
}
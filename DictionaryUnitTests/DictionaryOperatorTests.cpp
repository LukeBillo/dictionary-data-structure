#include "pch.h"
#include "../Dictionary/dictionary.h"

TEST(DictionaryEqualsOperatorTest, WhenEquatingToEqualDictionaries)
{
	auto* dictionary = new Containers::Dictionary<int, int>();
	dictionary->insert(0, 0);

	auto* equivalentDictionary = new Containers::Dictionary<int, int>();
	equivalentDictionary->insert(0, 0);

	EXPECT_TRUE(*dictionary == *equivalentDictionary);
}
#include "pch.h"
#include "gtest/gtest.h"
#include "../Dictionary/dictionary.h"

TEST(DictionaryCopyConstructorTest, WhenCopyingAnEmptyDictionary) {
	auto* emptyDictionary = new Containers::Dictionary<std::string, int>();
	auto* copiedDictionary = new Containers::Dictionary<std::string, int>(*emptyDictionary);

	EXPECT_EQ(*emptyDictionary, *copiedDictionary);
}

TEST(DictionaryCopyConstructorTest, WhenCopyingANonEmptyDictionary) {
	auto* originalDictionary = new Containers::Dictionary<std::string, int>();
	originalDictionary->insert("Test-1", 1);
	originalDictionary->insert("Test-2", 2);

	auto* copiedDictionary = new Containers::Dictionary<std::string, int>(*originalDictionary);
	EXPECT_EQ(*originalDictionary, *copiedDictionary);

	// check pointer addresses not equal; should be deep copy
	auto* originalAddress = originalDictionary->lookup("Test-1");
	auto* copyAddress = copiedDictionary->lookup("Test-1");

	EXPECT_NE(originalAddress, copyAddress);
}

TEST(DictionaryMoveConstructorTest, WhenMovingANonEmptyDictionary)
{
	auto* dictionaryToBeMoved = new Containers::Dictionary<std::string, int>();
	dictionaryToBeMoved->insert("Moved-1", 1);
	dictionaryToBeMoved->insert("Moved-2", 2);

	Containers::Dictionary<std::string, int> movedDictionary = std::move(*dictionaryToBeMoved);

	auto* shouldNotBeFound = dictionaryToBeMoved->lookup("Moved-1");
	auto* shouldBeMovedItem = movedDictionary.lookup("Moved-1");

	EXPECT_EQ(shouldNotBeFound, nullptr);
	EXPECT_EQ(*shouldBeMovedItem, 1);
}
#pragma once
#include <ostream>

namespace Containers
{
	template <class K, class I>
	class Dictionary
	{
	public:
		/* Types */
		struct KeyItemPair;

		using Key = K;
		using Item = I;
		using RemovePredicate = auto (Key)->bool;

		/* Constructors and Destructors */

		// Deep copy constructor
		Dictionary(const Dictionary<Key, Item>&);

		// Move constructor
		Dictionary(Dictionary<Key, Item>&&) noexcept;

		// Default constructor and destructor
		Dictionary();
		~Dictionary();

		/* Operator overloads */

		template<class T1, class T2>
		friend std::ostream& operator<<(std::ostream&, const Dictionary<T1, T2>&);

		/**
		 * I used this for testing purposes (i.e. dictionary1 == dictionary2),
		 * but left it in the API since it's useful anyway.
		 */
		template<class T1, class T2>
		friend bool operator==(Dictionary<T1, T2> const&, Dictionary<T1, T2> const&);

		Dictionary<Key, Item>& operator=(Dictionary<Key, Item> const&);
		Dictionary<Key, Item>& operator=(Dictionary<Key, Item> &&) noexcept;

		/* Public, Non-Static Functions */

		bool insert(Key, Item);
		Item* lookup(Key);
		bool remove(Key);
		int removeIf(RemovePredicate);
		std::ostream& display(std::ostream&) const;

	private:
		std::unique_ptr<KeyItemPair> _root;

		/* Constants */
		static const int DICTIONARY_OUTPUT_INDENTATION = 2;

		/* Helper functions for internal use */
		static int deepDelete(std::unique_ptr<KeyItemPair> &);
		static void removePair(std::unique_ptr<KeyItemPair> &);
		static bool isEmpty(const std::unique_ptr<KeyItemPair>&);

		/* Recursive helper functions for internal use */
		static Item* lookupRecursive(Key, std::unique_ptr<KeyItemPair>&);
		static void displayDictionaryRecursive(std::ostream&, const std::unique_ptr<KeyItemPair>&);
		static bool insertRecursive(Key, Item, std::unique_ptr<KeyItemPair>&);
		static std::unique_ptr<KeyItemPair> deepCopyRecursive(const std::unique_ptr<KeyItemPair>&);
		static int deepDeleteRecursive(std::unique_ptr<KeyItemPair>&, int);
		static bool removeRecursive(Key, std::unique_ptr<KeyItemPair>&);
		static int removeIfRecursive(RemovePredicate, std::unique_ptr<KeyItemPair> &, int);
		static bool checkIfEqualRecursive(std::unique_ptr<KeyItemPair> const&, std::unique_ptr<KeyItemPair> const&);
	};

	/**
	 * Used internally as a 'node' of the singly
	 * linked list.
	 * Externally, this is a key-item pair in
	 * the dictionary, hence the name.
	 */
	template <class K, class I>
	struct Dictionary<K, I>::KeyItemPair
	{
		Key key;
		Item item;
		std::unique_ptr<KeyItemPair> nextPair;

		KeyItemPair(Key key, Item item) : key(key), item(item), nextPair(std::unique_ptr<KeyItemPair>(nullptr)) {}
	};

	template <class K, class I>
	Dictionary<K, I>::Dictionary(const Dictionary<Key, Item>& existingDictionary)
	{
		this->_root = deepCopyRecursive(existingDictionary._root);
	}

	template <class K, class I>
	Dictionary<K, I>::Dictionary(Dictionary<Key, Item>&& moveFrom) noexcept
	{
		if (this == &moveFrom)
		{
			return;
		}

		_root = std::move(moveFrom._root);
	}

	template <class K, class I>
	Dictionary<K, I>::Dictionary()
	{
		_root = std::unique_ptr<KeyItemPair>(nullptr);
	}

	template <class K, class I>
	Dictionary<K, I>::~Dictionary()
	{
		deepDeleteRecursive(_root, 0);
	}

	template <class K, class I>
	Dictionary<typename Dictionary<K, I>::Key, typename Dictionary<K, I>::Item>& Dictionary<K, I>::operator=(
		Dictionary<Key, Item> const& copyDictionary)
	{
		if (this == copyDictionary)
			return *this;

		deepDeleteRecursive(this->_root);
		_root = deepCopyRecursive(copyDictionary._root);

		return *this;
	}

	template <class K, class I>
	Dictionary<typename Dictionary<K, I>::Key, typename Dictionary<K, I>::Item>& Dictionary<K, I>::operator=(
		Dictionary<Key, Item>&& moveFrom) noexcept
	{
		if (this == &moveFrom)
		{
			return *this;
		}
		
		_root = std::move(moveFrom._root);

		return *this;
	}

	/**
	 * Returns a boolean representing if the
	 * key didn't exist prior to the insert.
	 */
	template <class K, class I>
	bool Dictionary<K, I>::insert(Key key, Item itemToInsert)
	{
		return insertRecursive(key, itemToInsert, _root);
	}

	template <class K, class I>
	bool Dictionary<K, I>::insertRecursive(Key key, Item itemToInsert, std::unique_ptr<KeyItemPair>& currentPair)
	{
		// at an empty node, so insert
		// return true because key didn't exist
		if (isEmpty(currentPair))
		{
			currentPair = std::make_unique<KeyItemPair>(key, itemToInsert);
			return true;
		}

		// key already exists; replace the item
		// and return false
		if (currentPair->key == key)
		{
			currentPair->item = itemToInsert;
			return false;
		}

		// else, go to the next pair in the singly linked list
		return insertRecursive(key, itemToInsert, currentPair->nextPair);
	}

	template <class K, class I>
	std::unique_ptr<typename Dictionary<K, I>::KeyItemPair> Dictionary<K, I>::deepCopyRecursive(
		const std::unique_ptr<KeyItemPair>& copyPair)
	{
		if (isEmpty(copyPair))
			return std::unique_ptr<KeyItemPair>(nullptr);

		std::unique_ptr<KeyItemPair> copiedPair = std::make_unique<KeyItemPair>(copyPair->key, copyPair->item);
		copiedPair->nextPair = deepCopyRecursive(copyPair->nextPair);

		return copiedPair;
	}

	/**
	 * Returns a Key's corresponding Item.
	 * If the Key isn't found, nullptr is returned.
	 */
	template <class K, class I>
	typename Dictionary<K, I>::Item* Dictionary<K, I>::lookup(Key keyToFind)
	{
		return lookupRecursive(keyToFind, _root);
	}

	template <class K, class I>
	typename Dictionary<K, I>::Item* Dictionary<K, I>::lookupRecursive(Key keyToFind, std::unique_ptr<KeyItemPair>& currentPair)
	{
		// end of the line, the key is absent
		if (isEmpty(currentPair))
		{
			return nullptr;
		}

		// found the key
		if (currentPair->key == keyToFind)
		{
			return &currentPair->item;
		}

		// otherwise, keep going
		return lookupRecursive(keyToFind, currentPair->nextPair);
	}

	/**
	 * Returns the number of nodes deleted, including the
	 * starting node.
	 */
	template <class K, class I>
	int Dictionary<K, I>::deepDelete(std::unique_ptr<KeyItemPair> & firstPair)
	{
		return deepDeleteRecursive(firstPair, 0);
	}

	template <class K, class I>
	int Dictionary<K, I>::deepDeleteRecursive(std::unique_ptr<KeyItemPair> & currentPair, int numDeletedPairs)
	{
		// if no current pair, return how many were deleted
		if (isEmpty(currentPair))
		{
			return numDeletedPairs;
		}

		// otherwise, keep going down until reaches empty pair
		numDeletedPairs = deepDeleteRecursive(currentPair->nextPair, numDeletedPairs + 1);

		// deletes non-empty pairs, starting from the last pair
		// cascades backwards after due to recursion
		currentPair = nullptr;

		return numDeletedPairs;
	}

	/**
	 * Returns a boolean representing whether the
	 * key existed. 
	 * (i.e. was a key-item pair removed?)
	 */
	template <class K, class I>
	bool Dictionary<K, I>::remove(Key keyToRemove)
	{
		return removeRecursive(keyToRemove, _root);
	}

	template <class K, class I>
	bool Dictionary<K, I>::removeRecursive(Key keyToRemove, std::unique_ptr<KeyItemPair> & currentPair)
	{
		// key doesnt exist, return false to
		// indicate no key-item pair was removed
		if (isEmpty(currentPair))
		{
			return false;
		}

		// found the key, remove and return true
		if (currentPair->key == keyToRemove)
		{
			removePair(currentPair);
			return true;
		}

		// else, keep going
		return removeRecursive(keyToRemove, currentPair->nextPair);
	}

	/**
	 * Removes the provided key-item pair if not empty.
	 */
	template <class K, class I>
	void Dictionary<K, I>::removePair(std::unique_ptr<KeyItemPair> & currentPair)
	{
		if (isEmpty(currentPair))
			return;

		currentPair = std::move(currentPair->nextPair);
	}

	/**
	 * Takes a predicate and removes all existing keys that
	 * match the predicate.
	 * Returns the number of key-item pairs removed.
	 */
	template <class K, class I>
	int Dictionary<K, I>::removeIf(RemovePredicate predicate)
	{
		return removeIfRecursive(predicate, _root, 0);
	}

	template <class K, class I>
	int Dictionary<K, I>::removeIfRecursive(RemovePredicate predicate, std::unique_ptr<KeyItemPair>& currentPair, int removedCounter)
	{
		// end of the list, so return the number of
		// removed key-item pairs
		if (isEmpty(currentPair))
		{
			return removedCounter;
		}

		// otherwise, test the current pair's key
		// against the predicate; increment
		// counter if removed pair...
		if (predicate(currentPair->key))
		{
			removePair(currentPair);
			removedCounter++;

			// due to reassignment of pointers, now need to check the
			// current pair again
			return removeIfRecursive(predicate, currentPair, removedCounter);
		}

		// and then go to the next...
		return removeIfRecursive(predicate, currentPair->nextPair, removedCounter);
	}

	template <class K, class I>
	bool Dictionary<K, I>::isEmpty(const std::unique_ptr<KeyItemPair>& pair)
	{
		return !pair;
	}

	/**
	 * Outputs dictionary in a display-friendly format
	 * to any stream.
	 * 
	 * Example:
	 * {
	 *   { TestKey : TestValue }
	 *   { AnotherTestKey : AnotherTestValue }
	 * }
	 * 
	 * Assumes that both K and I have the operator overload
	 * for operator<<.
	 */
	template <class K, class I>
	std::ostream& Dictionary<K, I>::display(std::ostream& os) const
	{
		os << "{" << std::endl;
		displayDictionaryRecursive(os, _root);
		os << "}" << std::endl;

		return os;
	}

	template <class K, class I>
	void Dictionary<K, I>::displayDictionaryRecursive(std::ostream& os, const std::unique_ptr<KeyItemPair>& currentPair)
	{
		if (isEmpty(currentPair))
		{
			return;
		}

		// indent with spaces
		for (auto i = 0; i < DICTIONARY_OUTPUT_INDENTATION; ++i)
		{
			os << " ";
		}

		os << "{ " << currentPair->key << " : " << currentPair->item << " }" << std::endl;

		displayDictionaryRecursive(os, currentPair->nextPair);
	}

	template <class K, class I>
	bool Dictionary<K, I>::checkIfEqualRecursive(std::unique_ptr<KeyItemPair> const& lhsPair, std::unique_ptr<KeyItemPair> const& rhsPair)
	{
		/**
		* If either pair is nullptr, check if both are nullptr.
		* Ensures dictionary length (i.e. number of
		* KeyItemPairs is the same.)
		*/
		if (isEmpty(lhsPair) || isEmpty(rhsPair))
		{
			return isEmpty(lhsPair) && isEmpty(rhsPair);
		}

		// check if keys are equal
		if (lhsPair->key != rhsPair->key)
		{
			return false;
		}

		// check items are equal
		if (lhsPair->item != rhsPair->item)
		{
			return false;
		}

		// and then check the next pair
		return checkIfEqualRecursive(lhsPair->nextPair, rhsPair->nextPair);
	}

	/**
	 * Assumes that types K and I both
	 * have overloads for operator<<.
	 */
	template<class K, class I>
	std::ostream & operator<<(std::ostream & os, const Dictionary<K, I>& outputDictionary)
	{
		return outputDictionary.display(os);
	}

	template<class K, class I>
	bool operator==(Dictionary<K, I> const& lhs, Dictionary<K, I> const& rhs)
	{
		return Dictionary<K, I>::checkIfEqualRecursive(lhs._root, rhs._root);
	}
}

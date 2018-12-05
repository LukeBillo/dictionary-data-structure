#pragma once

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
		Dictionary(const Dictionary<Key, Item>&);
		Dictionary(Dictionary<Key, Item>&&) noexcept;
		Dictionary() = default;
		~Dictionary();

		Dictionary<Key, Item>& operator=(Dictionary<Key, Item> const&);
		Dictionary<Key, Item>& operator=(Dictionary<Key, Item> &&) noexcept;

		/* Public, Non-Static Functions */
		bool insert(Key, Item);
		Item* lookup(Key);
		bool remove(Key);
		int removeIf(RemovePredicate);

	private:
		KeyItemPair* _root = nullptr;

		static bool insert_recursive(Key, Item, KeyItemPair* &);
		static Item* lookup_recursive(Key, KeyItemPair*);
		static bool remove_recursive(Key, KeyItemPair* &);
		static void remove_pair(KeyItemPair* &);
		static int remove_if_recursive(RemovePredicate, KeyItemPair* &, int);
		static bool is_empty(const KeyItemPair*);
	};

	template <class K, class I>
	struct Dictionary<K, I>::KeyItemPair
	{
		Key key;
		Item item;
		KeyItemPair* next_pair;

		KeyItemPair(Key key, Item item) : key(key), item(item), next_pair(nullptr) {}
	};

	template <class K, class I>
	Dictionary<K, I>::Dictionary(const Dictionary<Key, Item>& existing_dictionary)
	{
	}

	template <class K, class I>
	Dictionary<K, I>::Dictionary(Dictionary<Key, Item>&& existing_dictionary) noexcept
	{
	}

	template <class K, class I>
	Dictionary<K, I>::~Dictionary()
	{
	}

	/**
	 * Returns a boolean representing if the
	 * key didn't exist prior to the insert.
	 */
	template <class K, class I>
	bool Dictionary<K, I>::insert(Key key, Item item_to_insert)
	{
		return insert_recursive(key, item_to_insert, _root);
	}

	template <class K, class I>
	bool Dictionary<K, I>::insert_recursive(Key key, Item item_to_insert, KeyItemPair* & current_pair)
	{
		// at an empty node, so insert
		// return true because key didn't exist
		if (is_empty(current_pair))
		{
			current_pair = new KeyItemPair(key, item_to_insert);
			return true;
		}

		// key already exists; replace the item
		// and return false
		if (current_pair->key == key)
		{
			current_pair->item = item_to_insert;
			return false;
		}

		// else, go to the next pair in the singly linked list
		return insert_recursive(key, item_to_insert, current_pair->next_pair);
	}

	/**
	 * Returns a Key's corresponding Item.
	 * If the Key isn't found, nullptr is returned.
	 */
	template <class K, class I>
	typename Dictionary<K, I>::Item* Dictionary<K, I>::lookup(Key key_to_find)
	{
		return lookup_recursive(key_to_find, _root);
	}

	template <class K, class I>
	typename Dictionary<K, I>::Item* Dictionary<K, I>::lookup_recursive(Key key_to_find, KeyItemPair* current_pair)
	{
		// end of the line, the key is absent
		if (is_empty(current_pair))
		{
			return nullptr;
		}

		// found the key
		if (current_pair->key == key_to_find)
		{
			return &current_pair->item;
		}

		// otherwise, keep going
		return lookup_recursive(key_to_find, current_pair->next_pair);
	}

	/**
	 * Returns a boolean representing whether the
	 * key existed. 
	 * (i.e. was a key-item pair removed?)
	 */
	template <class K, class I>
	bool Dictionary<K, I>::remove(Key key_to_remove)
	{
		return remove_recursive(key_to_remove, _root);
	}

	template <class K, class I>
	bool Dictionary<K, I>::remove_recursive(Key key_to_remove, KeyItemPair* & current_pair)
	{
		// key doesnt exist, return false to
		// indicate no key-item pair was removed
		if (is_empty(current_pair))
		{
			return false;
		}

		// found the key, remove and return true
		if (current_pair->key == key_to_remove)
		{
			remove_pair(current_pair);
			return true;
		}

		// else, keep going
		return remove_recursive(key_to_remove, current_pair->next_pair);
	}

	/**
	 * Removes the provided key-item pair if not empty.
	 */
	template <class K, class I>
	void Dictionary<K, I>::remove_pair(KeyItemPair* & current_pair)
	{
		if (is_empty(current_pair))
			return;

		KeyItemPair* next_pair = current_pair->next_pair;
		delete current_pair;
		current_pair = next_pair;
	}

	/**
	 * Takes a predicate and removes all existing keys that
	 * match the predicate.
	 * Returns the number of key-item pairs removed.
	 */
	template <class K, class I>
	int Dictionary<K, I>::removeIf(typename Dictionary<K, I>::RemovePredicate predicate)
	{
		return remove_if_recursive(predicate, _root, 0);
	}

	template <class K, class I>
	int Dictionary<K, I>::remove_if_recursive(RemovePredicate predicate, KeyItemPair*& current_pair, int removed_counter)
	{
		// end of the list, so return the number of
		// removed key-item pairs
		if (is_empty(current_pair))
		{
			return removed_counter;
		}

		// otherwise, test the current pair's key
		// against the predicate; increment
		// counter if removed pair...
		if (predicate(current_pair->key))
		{
			remove_pair(current_pair);
			removed_counter++;
		}

		// and then go to the next...
		return remove_if_recursive(predicate, current_pair->next_pair, removed_counter);
	}

	template <class K, class I>
	bool Dictionary<K, I>::is_empty(const KeyItemPair* pair)
	{
		return pair == nullptr;
	}
}



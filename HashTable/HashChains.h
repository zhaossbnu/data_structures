#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <functional>
#include <forward_list>
#include <iterator>
template<typename K, typename E>
class HashChains
{
public:
	HashChains(int divisor);

	std::pair<const K, E>* find(const K &key) const;
	void insert(std::pair<const K, E> &in_pair);
	void earse(const K &key);

private:
	int divisor;
	int d_size;
	std::vector<std::forward_list<std::pair<const K, E>>*> table;
};

template<typename K, typename E>
HashChains<K, E>::HashChains(int divisor)
{
	d_size = 0;
	this->divisor = divisor;
	table.resize(divisor);
	for (int i = 0; i < divisor; ++i)
	{
		table[i] = NULL;
	}
}


template<typename K, typename E>
std::pair<const K, E>* HashChains<K, E>::find(const K &key) const
{
	for (auto it = table[key % divisor]->cbegin(); it != table[key % divisor]->cend(); ++it)
	{
		if (it->first == key)
		{
			return it;
		}
	}
	return NULL;
}

template<typename K, typename E>
void HashChains<K, E>::insert(std::pair<const K, E>& in_pair)
{
	int bucket = (int)std::hash(in_pair.first);
	int old_size = table[bucket]->size();
	std::pair<const K, E> *current = find(in_pair.first);
	if (current != NULL)
	{
		current->second = in_pair.second;
	}
	else
	{
		table[bucket]->push_front(in_pair);
		++d_size;
	}
}

template<typename K, typename E>
void HashChains<K, E>::earse(const K & key)
{
	for (auto it = table[key % divisor]->begin(); it != table[key % divisor]->end(); ++it)
	{
		if (it->next->first == key)
		{
			table[key % divisor]->earse(it);
			--d_size;
			return;
		}
	}
} 

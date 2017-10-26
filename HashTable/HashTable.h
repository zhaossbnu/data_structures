#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <functional>
template<typename K, typename E>
class HashTable
{
public:
	HashTable(int divisor);
	int search(const K &key) const;
	std::pair<const K, E>* find(const K &key) const;
	void insert(const std::pair<const K, E> &in_pair);
private:
	std::vector<std::pair<const K, E>*> table;
	std::hash<K> hash;
	int d_size;
	int divisor; // 散列函数除数

};

template<typename K, typename E>
HashTable<K, E>::HashTable(int divisor)
{
	this->divisor = divisor;
	d_size = 0;
	table.resize(divisor);
	for (int i = 0; i < divisor; ++i)
	{
		table[i] = NULL;
	}
}

template<typename K, typename E>
int HashTable<K, E>::search(const K & key) const
{
	int i = (int)hash(key) % divisor;
	int j = i;
	do
	{
		// 如果存在 返回地址
		// 如果不存在 返回空位置地址 方便插入
		if (table[j] == NULL || table[j]->first == key)
		{
			return j;
		}
		j = (j + 1) % divisor;
	} while (j != i);
	return j;
}

template<typename K, typename E>
std::pair<const K, E>* HashTable<K, E>::find(const K & key) const
{
	// 查询位置
	int b = search(key);

	// 查询位置为空 或者 查询位置上的数据不是要找的 表明没有找到
	if (table[b] == NULL || table[b]->first != key)
	{
		return NULL;
	}

	return table[b];
}

template<typename K, typename E>
void HashTable<K, E>::insert(const std::pair<const K, E>& in_pair)
{
	// 查找插入位置
	int b = search(in_pair.first);
	// 有空位置
	if (table[b] == NULL)
	{
		table[b] = new std::pair<const K, E>(in_pair);
		++d_size;
	}
	else // 没有空位置
	{
		// 当前位置已有同键值的数据  覆盖之
		if (table[b]->first == in_pair.first)
		{
			table[b]->second = in_pair.second;
		}
		else // 当前位置已经有其他数据 表明表已经满了
		{
			std::cerr << "Table Full!" << endl;
		}
	}
}

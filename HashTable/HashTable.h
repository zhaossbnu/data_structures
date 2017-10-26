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
	int divisor; // ɢ�к�������

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
		// ������� ���ص�ַ
		// ��������� ���ؿ�λ�õ�ַ �������
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
	// ��ѯλ��
	int b = search(key);

	// ��ѯλ��Ϊ�� ���� ��ѯλ���ϵ����ݲ���Ҫ�ҵ� ����û���ҵ�
	if (table[b] == NULL || table[b]->first != key)
	{
		return NULL;
	}

	return table[b];
}

template<typename K, typename E>
void HashTable<K, E>::insert(const std::pair<const K, E>& in_pair)
{
	// ���Ҳ���λ��
	int b = search(in_pair.first);
	// �п�λ��
	if (table[b] == NULL)
	{
		table[b] = new std::pair<const K, E>(in_pair);
		++d_size;
	}
	else // û�п�λ��
	{
		// ��ǰλ������ͬ��ֵ������  ����֮
		if (table[b]->first == in_pair.first)
		{
			table[b]->second = in_pair.second;
		}
		else // ��ǰλ���Ѿ����������� �������Ѿ�����
		{
			std::cerr << "Table Full!" << endl;
		}
	}
}

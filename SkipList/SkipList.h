#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <cstdlib>
template <typename K, typename E>
struct SkipNode
{
	std::pair<const K, E> element;
	std::vector<SkipNode<K, E>*> next;

	SkipNode(const std::pair<const K, E>, int size)
	{
		element = pair;
		next.resize(size);
	}
};

template <typename K, typename E>
class SkipList
{
public:
	SkipList(K largerest_key, int max_pairs, double prob);
	~SkipList()
	{

	}
	std::pair<const K, E>* find(const K &key) const;
	int level() const;
	SkipNode<K, E>* search(const K &key) const;
	void insert(const std::pair<const K, E> &in_pair);
	void earse(const K &key);
private:
	double cut_off;
	int levels;
	int d_size;
	int max_level;
	K tail_key;
	SkipNode<K, E> *header;
	SkipNode<K, E> *tail;
	std::vector<SkipNode<K, E>*> last;
};

template<typename K, typename E>
SkipList<K, E>::SkipList(K largerest_key, int max_pairs, double prob)
{
	cut_off = prob * RAND_MAX;
	max_level = (int)ceil(log10(max_pairs) / log10(1 / prob)) - 1;
	levels = 0;  // ��ʼ������
	d_size = 0;
	tail_key = largerest_key;
	std::pair<K, E> tail_pair;
	tail_pair.first = tail_key;
	header = new SkipNode<K, E>(tail_pair, max_level + 1);
	tail = new SkipNode<K, E>(tail_pair, 0);
	last.resize(max_level + 1);
	// ����Ϊ��ʱ�� ���⼶�����е�ͷ��㶼ָ��β�ڵ�
	for (int i = 0; i <= max_level; ++i)
	{
		header->next[i] = tail;
	}
}

template<typename K, typename E>
std::pair<const K, E>* SkipList<K, E>::find(const K & key) const
{
	// ����ؼ����Ѿ�Խ��
	if (key == tail_key)
	{
		return NULL;
	}
	SkipNode<K, E> *before = header;
	// ���ռ����Ӵ�С��ѯ
	for (int i = levels; i >= 0; --i)
	{
		while (before->next[i]->element.first < key)
		{
			before = before->next[i];
		}
	}
	// �����ǰ�ڵ����һ���ڵ�Ĺؼ���ƥ�� ���鵽
	// ��Ȼ �����������Ե�ʱ���� 0����
	if (before->next[0]->element.first == key)
	{
		return &before->next[0]->element;
	}
	return NULL;
}

template<typename K, typename E>
int SkipList<K, E>::level() const
{
	//���ռ������䷽�����з���
	int lev = 0;
	while (rand() <= cut_off)
	{
		++lev;
	}
	// ��Ȼ �������ܳ��������
	return (lev <= max_level) ? lev : max_level;
}

template<typename K, typename E>
SkipNode<K, E>* SkipList<K, E>::search(const K & key) const
{
	// ��������Ŀ�ľ���Ϊ�˱�ʶlast
	SkipNode<K, E> *before = header;
	for (int i = levels; i >= 0; ++i)
	{
		while (before->next[i]->element.first < key)
		{
			before = before->next[i];
		}
		// ��ʶѰ�ҵ�ǰ�ڵ�ʱÿһ�����������һ���ڵ�
		// �����ʮ����Ҫ��һ��
		last[i] = before;
	}
	return before->next[0];
}

template<typename K, typename E>
void SkipList<K, E>::insert(const std::pair<const K, E>& in_pair)
{
	// ����ĵĹؼ��ֹ��� ����
	if (in_pair.first == tail_key)
	{
		std::cerr << "Key too big!" << endl;
		return;
	}
	// ��ʶ���ҵ�ǰ�ڵ�ʱ�� last
	SkipNode<K, E> *current = search(in_pair.first);
	// �����ǰ����Ĺؼ����ڱ��� ֻ���޸���ֵ����
	if (current->element.first == in_pair.first)
	{
		current->element->second = in_pair->second;
		return;
	}
	
	// �����Ҫִ�в������
	int get_level = level();
	// �������ļ������пռ��� ����������Ϊδʹ�õļ�������С��
	if (get_level > levels)
	{
		get_level = ++levels;
		// ע�� ���¼����lastҲҪ���� ���ó� header
		last[get_level] = header;
	}

	ShipNode<K, E> *new_node = new SkipNode<K, E>(in_pair, get_level + 1);
	// ˢ������
	for (int i = 0; i <= get_level; ++i)
	{
		new_node->next[i] = last[i]->next[i];
		last[i]->next[i] = new_node;
	}
	// ��������Ԫ�ظ���
	++d_size;
}

template<typename K, typename E>
inline void SkipList<K, E>::earse(const K & key)
{
	// ��ɾ��������
	if (key >= tail_key)
	{
		return;
	}
	// ���ô�ɾ���Ե�last
	SkipNode<K, E> *current = search(key);
	// �����������
	if (current->element.first != key)
	{
		return;
	}
	// ������ҵ��ĵ�ǰ�ڵ����Ҫɾ����
	// ˢ������
	for (int i = 0; i < levels && last[i]->next[i] == current; ++i)
	{
		last[i]->next[i] = current->next[i];
	}
	// ɾ���ڵ���ܻᵼ��һЩ�����ǿյ�
	// ����Ҫ��������ǰ����
	while (levels > 0 && header->next[levels] == tail)
	{
		--levels;
	}
	// һ��Ҫ�ͷ��ڴ�
	delete current;
	--d_size;
}



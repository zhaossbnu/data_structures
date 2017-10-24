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
	levels = 0;  // 初始化级数
	d_size = 0;
	tail_key = largerest_key;
	std::pair<K, E> tail_pair;
	tail_pair.first = tail_key;
	header = new SkipNode<K, E>(tail_pair, max_level + 1);
	tail = new SkipNode<K, E>(tail_pair, 0);
	last.resize(max_level + 1);
	// 链表为空时， 任意级链表中的头结点都指向尾节点
	for (int i = 0; i <= max_level; ++i)
	{
		header->next[i] = tail;
	}
}

template<typename K, typename E>
std::pair<const K, E>* SkipList<K, E>::find(const K & key) const
{
	// 如果关键字已经越界
	if (key == tail_key)
	{
		return NULL;
	}
	SkipNode<K, E> *before = header;
	// 按照级数从大到小查询
	for (int i = levels; i >= 0; --i)
	{
		while (before->next[i]->element.first < key)
		{
			before = before->next[i];
		}
	}
	// 如果当前节点的下一个节点的关键字匹配 即查到
	// 当然 真正返回数对的时候是 0级的
	if (before->next[0]->element.first == key)
	{
		return &before->next[0]->element;
	}
	return NULL;
}

template<typename K, typename E>
int SkipList<K, E>::level() const
{
	//按照级数分配方法进行分配
	int lev = 0;
	while (rand() <= cut_off)
	{
		++lev;
	}
	// 当然 级数不能超过最大级数
	return (lev <= max_level) ? lev : max_level;
}

template<typename K, typename E>
SkipNode<K, E>* SkipList<K, E>::search(const K & key) const
{
	// 本函数的目的就是为了标识last
	SkipNode<K, E> *before = header;
	for (int i = levels; i >= 0; ++i)
	{
		while (before->next[i]->element.first < key)
		{
			before = before->next[i];
		}
		// 标识寻找当前节点时每一级遇到的最后一个节点
		// 这个是十分重要的一步
		last[i] = before;
	}
	return before->next[0];
}

template<typename K, typename E>
void SkipList<K, E>::insert(const std::pair<const K, E>& in_pair)
{
	// 插入的的关键字过大 抛弃
	if (in_pair.first == tail_key)
	{
		std::cerr << "Key too big!" << endl;
		return;
	}
	// 标识查找当前节点时的 last
	SkipNode<K, E> *current = search(in_pair.first);
	// 如果当前插入的关键字在表中 只用修改其值即可
	if (current->element.first == in_pair.first)
	{
		current->element->second = in_pair->second;
		return;
	}
	
	// 否则就要执行插入操作
	int get_level = level();
	// 如果分配的级数下有空级数 将级数设置为未使用的级别中最小的
	if (get_level > levels)
	{
		get_level = ++levels;
		// 注意 最新级别的last也要设置 设置成 header
		last[get_level] = header;
	}

	ShipNode<K, E> *new_node = new SkipNode<K, E>(in_pair, get_level + 1);
	// 刷新跳表
	for (int i = 0; i <= get_level; ++i)
	{
		new_node->next[i] = last[i]->next[i];
		last[i]->next[i] = new_node;
	}
	// 增加跳表元素个数
	++d_size;
}

template<typename K, typename E>
inline void SkipList<K, E>::earse(const K & key)
{
	// 待删除键过大
	if (key >= tail_key)
	{
		return;
	}
	// 设置待删除对的last
	SkipNode<K, E> *current = search(key);
	// 如果键不存在
	if (current->element.first != key)
	{
		return;
	}
	// 如果查找到的当前节点就是要删除的
	// 刷新跳表
	for (int i = 0; i < levels && last[i]->next[i] == current; ++i)
	{
		last[i]->next[i] = current->next[i];
	}
	// 删除节点可能会导致一些级别是空的
	// 所以要更新跳表当前级别
	while (levels > 0 && header->next[levels] == tail)
	{
		--levels;
	}
	// 一定要释放内存
	delete current;
	--d_size;
}



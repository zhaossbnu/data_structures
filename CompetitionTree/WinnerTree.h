#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
template <typename T>
class WinnerTree
{
public:
	WinnerTree(const std::vector<T> &p);	
	int winner();
	void re_play(int i, const T &t);
private:
	std::vector<T> player;
	std::vector<int> tree;
	// s 是最后一层节点个数 n 是选手个数
	// 最底层外部节点个数low_ext = 2^(log2(n - 1))
	int low_ext;
	// 为了使得最后一层外部节点能找到内部节点 设置的偏移量
	// offset = 2 * s - 1
	int offset;
	// 进行一次比赛 p是父节点 left 是左边选手 right是右边选手
	void play(int p, int left, int right);
	void init();
};

template<typename T>
inline WinnerTree<T>::WinnerTree(const std::vector<T> &p)
{
	player.resize(p.size() + 1);
	std::copy(p.cbegin(), p.cend(), ++player.begin());
	init();
	
}

template<typename T>
inline int WinnerTree<T>::winner()
{
	return tree[1];
}

template<typename T>
inline void WinnerTree<T>::re_play(int p, const T &t)
{
	player[p] = t;
	int n = player.size() - 1;

	int parent, left, right;
	// 找到重新比赛的节节点
	if (p <= low_ext)
	{
		// 位于 最后一层节点区
		parent = (p + offset) / 2;
		left = 2 * parent - offset;
		right = left + 1;
	}
	else
	{
		parent = (p - low_ext + n - 1) / 2;
		if (2 * parent == n - 1)
		{
			// 重新比赛的节点的兄弟是内部节点
			left = tree[2 * parent];
			right = p;
		}
		else
		{
			left = 2 * parent + low_ext - n + 1;
			right = left + 1;
		}
	}

	tree[parent] = (player[left] <= player[right]) ? left : right;

	// 向上比赛到根
	parent /= 2;
	for (;parent >= 1; parent /= 2)
	{
		tree[parent] = (player[tree[2 * parent]] <= player[tree[2 * parent + 1]]) ? tree[2 * parent] : tree[2 * parent + 1];
	}

}

template<typename T>
inline void WinnerTree<T>::play(int p, int left, int right)
{
	tree[p] = (player[left] <= player[right]) ? left : right;
	// 比赛是右孩子驱动 也就是当右孩子完成之后比赛才能进行
	// 实质是个后序遍历
	while(p % 2 == 1 && p > 1)
	{
		tree[p / 2] = (player[tree[p - 1]] <= player[tree[p]]) ? tree[p - 1] : tree[p];
		p /= 2;
	}
}

template<typename T>
inline void WinnerTree<T>::init()
{
	int n = player.size() - 1;
	tree.resize(n);
	
	int s = (int)std::pow(2, std::floor(std::log2(n - 1)));
	low_ext = 2 * (n - s);
	offset = 2 * s - 1;

	int i;
	// 处理 最后一层节点
 	for (i = 2; i <= low_ext; i += 2)
	{
		play((i + offset) / 2, i - 1, i);
	}

	// 处理跨界特殊节点
	if (n % 2 == 1)
	{
		// n为奇数 说明跨界区有特殊点即其兄弟是一个内部节点
		play(n / 2, tree[n - 1], low_ext + 1);
		i = low_ext + 3;
	}
	else
	{
		// n为偶数 正常
		i = low_ext + 2;
	}
	// 处理剩余外部节点
	for (; i <= n; i += 2)
	{
		play((i - low_ext + n - 1) / 2, i - 1, i);
	}
}

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
	// s �����һ��ڵ���� n ��ѡ�ָ���
	// ��ײ��ⲿ�ڵ����low_ext = 2^(log2(n - 1))
	int low_ext;
	// Ϊ��ʹ�����һ���ⲿ�ڵ����ҵ��ڲ��ڵ� ���õ�ƫ����
	// offset = 2 * s - 1
	int offset;
	// ����һ�α��� p�Ǹ��ڵ� left �����ѡ�� right���ұ�ѡ��
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
	// �ҵ����±����Ľڽڵ�
	if (p <= low_ext)
	{
		// λ�� ���һ��ڵ���
		parent = (p + offset) / 2;
		left = 2 * parent - offset;
		right = left + 1;
	}
	else
	{
		parent = (p - low_ext + n - 1) / 2;
		if (2 * parent == n - 1)
		{
			// ���±����Ľڵ���ֵ����ڲ��ڵ�
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

	// ���ϱ�������
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
	// �������Һ������� Ҳ���ǵ��Һ������֮��������ܽ���
	// ʵ���Ǹ��������
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
	// ���� ���һ��ڵ�
 	for (i = 2; i <= low_ext; i += 2)
	{
		play((i + offset) / 2, i - 1, i);
	}

	// ����������ڵ�
	if (n % 2 == 1)
	{
		// nΪ���� ˵�������������㼴���ֵ���һ���ڲ��ڵ�
		play(n / 2, tree[n - 1], low_ext + 1);
		i = low_ext + 3;
	}
	else
	{
		// nΪż�� ����
		i = low_ext + 2;
	}
	// ����ʣ���ⲿ�ڵ�
	for (; i <= n; i += 2)
	{
		play((i - low_ext + n - 1) / 2, i - 1, i);
	}
}

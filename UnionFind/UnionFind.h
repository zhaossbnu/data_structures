#pragma once
#include <vector>
class UnionFind
{
public:
	UnionFind(int size);
	int find(int i);
	void Union(int root1, int root2);
private:
	std::vector<int> parent;
};

inline UnionFind::UnionFind(int size)
{
	parent.resize(size);
	for (auto &t : parent)
	{
		t = -1;
	}
}

inline int UnionFind::find(int i)
{
	int root = i;
	while (parent[root] > 0)
	{
		root = parent[root];
	}

	int current = i;
	while (current != root)
	{
		int par = parent[current];
		parent[current] = root;
		current = par;
	}
	return root;
}

inline void UnionFind::Union(int root1, int root2)
{
	if (parent[root1] < parent[root2])
	{
		parent[root1] += parent[root2];
		parent[root2] = root1;
	}
	else
	{
		parent[root2] += parent[root1];
		parent[root1] = root2;
	}
}

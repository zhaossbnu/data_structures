#pragma once

#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include <vector>
#include <queue>
template<typename T>
struct Node
{
	using NodePtr = std::shared_ptr<Node<T>>;
	std::pair<int, T> element;
	NodePtr left_child, right_child;

	Node(const T &t)
	{
		element.first = 1;
		element.second = t;
		left_child = nullptr;
		right_child = nullptr;
	}

	Node(const Node &node)
	{
		element.first = node.element.first;
		element.second = node.element.second;
		left_child = node.left_child;
		right_child = node.right_child;
	}

	friend std::ostream& operator<<(std::ostream &out, const Node<T> &node)
	{
		out << node.element.second;
		return out;
	}
};

template <typename T>
class MaxHblt
{
public:
	using NodePtr = std::shared_ptr<Node<T>>;
	MaxHblt()
	{
		root = nullptr;
	}
	MaxHblt(const T &t)
	{
		root = std::make_shared<Node<T>>(t);
	}
	bool empty()
	{
		return !root;
	}
	NodePtr &get_root()
	{
		return  root;
	}
	void init(std::vector<T> elements);
	void meld(MaxHblt<T> &b);
	Node<T> top();
	void push(const T &e);
	void pop();
private:
	NodePtr root;
	void meld(NodePtr &a, NodePtr &b);
};

template<typename T>
inline void MaxHblt<T>::meld(NodePtr &a, NodePtr &b)
{
	if (!b)
	{
		return;
	}
	if (!a)
	{
		a = b;
		return;
	}
	if (a->element.second < b->element.second)
	{
		std::swap(a, b);
	}

	meld(a->right_child, b);
	if (!a->left_child)
	{
		a->left_child = a->right_child;
		a->right_child = nullptr;
		a->element.first = 1;
	}
	else
	{
		if (a->left_child->element.first < a->right_child->element.first)
		{
			std::swap(a->left_child, a->right_child);
		}
		a->element.first = a->right_child->element.first + 1;
	}
}

template<typename T>
inline void MaxHblt<T>::init(std::vector<T> elements)
{
	std::queue<NodePtr> q;
	for (T i : elements)
	{
		q.push(std::make_shared<Node<T>>(i));
	}
	while (q.size() > 1)
	{
		NodePtr a = q.front();
		q.pop();
		NodePtr b = q.front();
		q.pop();
		meld(a, b);
		q.push(a);
	}

	if (q.size() == 1)
	{
		root = q.front();
	}
}

template<typename T>
inline void MaxHblt<T>::meld(MaxHblt<T> &b)
{
	meld(root, b.get_root());
	b.root = nullptr;
}

template <typename T>
Node<T> MaxHblt<T>::top()
{
	Node<T> node = *root;
	node.left_child = nullptr;
	node.right_child = nullptr;
	return node;

}

template <typename T>
void MaxHblt<T>::push(const T &e)
{
	NodePtr p = std::make_shared<Node<T>>(e);
	meld(root, p);
}

template <typename T>
void MaxHblt<T>::pop()
{
	NodePtr left = root->left_child;
	NodePtr right = root->right_child;

	root = left;
	meld(root, right);
}

#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#define NodePtr std::shared_ptr<BinarySearchNode<K, E>>
template<typename K, typename E>
struct BinarySearchNode
{
	std::pair<K, E> element;
	NodePtr left_child;
	NodePtr right_child;
	BinarySearchNode(const std::pair<K, E> &data)
	{
		element = data;
		left_child = nullptr;
		right_child = nullptr;
	}
	friend std::ostream &operator<<(std::ostream &out, const BinarySearchNode<K, E> &node)
	{
		out << node.element.first << "\t" << node.element.second;
		return out;
	}
};

template<typename K, typename E>
class BinarySearchTree
{
public:
	BinarySearchTree()
	{

	}
	// 从vector中初始化二叉搜索树
	BinarySearchTree(const std::vector<std::pair<K, E>> &datas)
	{
		for (auto data : datas)
		{
			insert(data);
		}
	}
	//using NodePtr = std::shared_ptr<BinarySerachNode<K, E>>;
	// 搜索
	const std::shared_ptr<std::pair<K, E>> find(const K& key) const;
	// 插入
	void insert(const std::pair<K, E> & in_pair);
	// 删除
	void earse(const K &key);
	// 从小到大输出
	void ascend()
	{
		in_order_out(root);
	}
private:
	NodePtr root = nullptr;
	// 中序遍历
	void in_order_out(NodePtr tree)
	{
		if (tree != nullptr)
		{
			in_order_out(tree->left_child);
			std::cout << *tree << std::endl;
			in_order_out(tree->right_child);
		}
	}
};

template<typename K, typename E>
inline const std::shared_ptr<std::pair<K, E>> BinarySearchTree<K, E>::find(const K & key) const
{
	NodePtr current = root;
	while (current != nullptr)
	{
		if (key < current->element.first)
		{
			current = current->left_child;
		}
		else if (key > current->element.first)
		{
			current = current->right_child;
		}
		else
		{
			return std::make_shared<std::pair<K, E>>(current->element);
		}
	}
	return nullptr;
}

template<typename K, typename E>
void BinarySearchTree<K, E>::insert(const std::pair<K, E> &in_pair)
{
	NodePtr current = root;
	NodePtr parent = nullptr;
	// 找到待插入位置 以及相应的父节点
	while (current != nullptr)
	{
		parent = current;
		if (in_pair.first < current->element.first)
		{
			current = current->left_child;
		}
		else if (in_pair.first > current->element.first)
		{
			current = current->right_child;
		}
		else
		{
			// 如果有重复关键字 则直接修改节点对应的值
			current->element.second = in_pair.second;
			return;
		}
	}

	NodePtr new_node = std::make_shared<BinarySearchNode<K, E>>(in_pair);
	if (root != nullptr)
	{
		if (in_pair.first < parent->element.first)
		{
			parent->left_child = new_node;
		}
		else
		{
			parent->right_child = new_node;
		}
	}
	else
	{
		root = new_node;
	}
}

template<typename K, typename E>
void BinarySearchTree<K, E>::earse(const K &key)
{
	NodePtr current = root;
	NodePtr parent = nullptr;
	// 找到待删除位置 以及相应的父节点
	while (current != nullptr && current->element.first != key)
	{
		parent = current;
		if (key < current->element.first)
		{
			current = current->left_child;
		}
		else
		{
			current = current->right_child;
		}
	}
	// 如果没有找到删除节点
	if (current == nullptr)
	{
		std::cout << "No element whose key is " << key << std::endl;
		return;
	}

	// 待删除节点有两个孩子
	if (current->left_child != nullptr && current->right_child != nullptr)
	{
		NodePtr left_big = current->left_child;
		NodePtr left_big_parent = current;
		// 找出左子树中的最大元素 
		while (left_big->right_child != nullptr)
		{
			left_big_parent = left_big;
			left_big = left_big->right_child;
		}
		// 左子树中最大元素直接替换待删除节点
		current->element = left_big->element;
		// 删除左子树中最大元素
		// 如果左子树中最大元素是某一个节点的左孩子 说明最大元素节点只有一个孩子
		if (left_big_parent->left_child == left_big)
		{
			left_big_parent->left_child = left_big->left_child;
		}
		else
		{
			left_big_parent->right_child = nullptr;
		}

		//NodePtr right_small = current->right_child;
		//NodePtr right_small_parent = current;
		//// 找出右子树中的最小元素
		//while (right_small->left_child != nullptr)
		//{
		//	right_small_parent = right_small;
		//	right_small = right_small->left_child;
		//}
		//// 右子树中最小元素直接替换待删除节点
		//current->element = right_small->element;
		//// 删除左子树中最小元素
		//// 如果左子树中最小元素是某一个节点的左孩子 说明最小元素节点只有一个孩子
		//if (right_small_parent->left_child == right_small)
		//{
		//	right_small_parent->left_child = nullptr;
		//}
		//else
		//{
		//	right_small_parent->right_child = right_small->right_child;
		//}
	}
	else
	{
		// 待删除节点至多有一个节点
		// 直观上需要分成 待删除节点没有孩子 待删除节点只有一个孩子两种情况
		// 但是其实是可以统一的
		NodePtr temp = nullptr;
		if (current->left_child != nullptr)
		{
			temp = current->left_child;
		}
		else
		{
			temp = current->right_child;
		}

		if (current == root)
		{
			root = temp;
		}
		else
		{
			if (parent->left_child == current)
			{
				parent->left_child = temp;
			}
			else
			{
				parent->right_child = temp;
			}
		}
	}	
}

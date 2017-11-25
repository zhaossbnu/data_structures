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
	// ��vector�г�ʼ������������
	BinarySearchTree(const std::vector<std::pair<K, E>> &datas)
	{
		for (auto data : datas)
		{
			insert(data);
		}
	}
	//using NodePtr = std::shared_ptr<BinarySerachNode<K, E>>;
	// ����
	const std::shared_ptr<std::pair<K, E>> find(const K& key) const;
	// ����
	void insert(const std::pair<K, E> & in_pair);
	// ɾ��
	void earse(const K &key);
	// ��С�������
	void ascend()
	{
		in_order_out(root);
	}
private:
	NodePtr root = nullptr;
	// �������
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
	// �ҵ�������λ�� �Լ���Ӧ�ĸ��ڵ�
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
			// ������ظ��ؼ��� ��ֱ���޸Ľڵ��Ӧ��ֵ
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
	// �ҵ���ɾ��λ�� �Լ���Ӧ�ĸ��ڵ�
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
	// ���û���ҵ�ɾ���ڵ�
	if (current == nullptr)
	{
		std::cout << "No element whose key is " << key << std::endl;
		return;
	}

	// ��ɾ���ڵ�����������
	if (current->left_child != nullptr && current->right_child != nullptr)
	{
		NodePtr left_big = current->left_child;
		NodePtr left_big_parent = current;
		// �ҳ��������е����Ԫ�� 
		while (left_big->right_child != nullptr)
		{
			left_big_parent = left_big;
			left_big = left_big->right_child;
		}
		// �����������Ԫ��ֱ���滻��ɾ���ڵ�
		current->element = left_big->element;
		// ɾ�������������Ԫ��
		// ��������������Ԫ����ĳһ���ڵ������ ˵�����Ԫ�ؽڵ�ֻ��һ������
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
		//// �ҳ��������е���СԪ��
		//while (right_small->left_child != nullptr)
		//{
		//	right_small_parent = right_small;
		//	right_small = right_small->left_child;
		//}
		//// ����������СԪ��ֱ���滻��ɾ���ڵ�
		//current->element = right_small->element;
		//// ɾ������������СԪ��
		//// �������������СԪ����ĳһ���ڵ������ ˵����СԪ�ؽڵ�ֻ��һ������
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
		// ��ɾ���ڵ�������һ���ڵ�
		// ֱ������Ҫ�ֳ� ��ɾ���ڵ�û�к��� ��ɾ���ڵ�ֻ��һ�������������
		// ������ʵ�ǿ���ͳһ��
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

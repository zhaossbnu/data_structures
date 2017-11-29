#ifndef AVLTREE_AVLTREE_H
#define AVLTREE_AVLTREE_H

#include <iostream>
#include <utility>
#include <memory>
#include <vector>
#include <stack>
#include <algorithm>

#define AVLNodePtr std::shared_ptr<AVLNode<K, E>>

template <typename K, typename E>
struct AVLNode
{
	// �ڵ�ƽ������ ���������ĸ߶ȼ�ȥ������
	int bf;
	// ���ĸ߶� �����ڵ�ĸ߶�Ϊ1
	int height;
	std::pair<K, E> element;
	AVLNodePtr left_child;
	AVLNodePtr right_child;
	AVLNode(std::pair<K, E> data)
	{
		bf = 0;
		height = 1;
		element = data;
		left_child = right_child = nullptr;
	}
	friend std::ostream &operator<<(std::ostream &out, const AVLNode &node)
	{
		out << node.bf << "\t" << node.height << "\t" << node.element.first << "\t" << node.element.second;
		return out;
	}
};

template <typename K, typename E>
class AVLTree
{
public:
	AVLTree(const std::vector<std::pair<K, E>> &datas)
	{
		for (auto data : datas)
		{
			insert(data);
		}
	}
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
	AVLNodePtr root;
	// �������
	void in_order_out(const AVLNodePtr tree)
	{
		if (tree != nullptr)
		{
			in_order_out(tree->left_child);
			std::cout << *tree << std::endl;
			in_order_out(tree->right_child);
		}
	}
	// ��ȡĳһ�������ĸ߶�
	int get_height(const AVLNodePtr &node)
	{
		if (node == nullptr)
		{
			return 0;
		}
		else
		{
			return node->height;
		}
	}
	// ˢ�������߶�
	void flush_height(AVLNodePtr &node)
	{
		node->height = std::max(get_height(node->left_child), get_height(node->right_child)) + 1;
	}
	// ˢ������ƽ������
	void flush_bf(AVLNodePtr &node)
	{
		node->bf = get_height(node->left_child) - get_height(node->right_child);
	}
	// ����
	void rotate_left(AVLNodePtr &node);
	// �ҵ���
	void rotate_right(AVLNodePtr &node);
	// �������˫��
	void rotate_left_right(AVLNodePtr &node);
	// ���Һ���˫��
	void rotate_right_left(AVLNodePtr &node);
	// ����ĳһ����ʹ��ƽ��
	void rotate_node(AVLNodePtr &parent);
};

template <typename K, typename E>
void AVLTree<K, E>::rotate_left(AVLNodePtr &node)
{
	AVLNodePtr old_parent = node;
	node = node->right_child;
	old_parent->right_child = node->left_child;
	node->left_child = old_parent;
	flush_height(old_parent);
	flush_bf(old_parent);
	flush_height(node);
	flush_bf(node);
}

template <typename K, typename E>
void AVLTree<K, E>::rotate_right(AVLNodePtr &node)
{
	AVLNodePtr old_parent = node;
	node = node->left_child;
	old_parent->left_child = node->right_child;
	node->right_child = old_parent;
	flush_height(old_parent);
	flush_bf(old_parent);
	flush_height(node);
	flush_bf(node);
}

template <typename K, typename E>
void AVLTree<K, E>::rotate_left_right(AVLNodePtr &node)
{
	AVLNodePtr old_grandparent = node, old_parent = node->left_child;
	// ����
	node = old_parent->right_child;
	old_parent->right_child = node->left_child;
	node->left_child = old_parent;
	flush_height(old_parent);
	flush_bf(old_parent);
	// ����
	old_grandparent->left_child = node->right_child;
	node->right_child = old_grandparent;
	flush_height(old_grandparent);
	flush_bf(old_grandparent);

	flush_height(node);
	flush_bf(node);
}


template <typename K, typename E>
void AVLTree<K, E>::rotate_right_left(AVLNodePtr &node)
{
	AVLNodePtr old_grandparent = node, old_parent = node->right_child;
	// ����
	node = old_parent->left_child;
	old_parent->left_child = node->right_child;
	node->right_child = old_parent;
	flush_height(old_parent);
	flush_bf(old_parent);
	// ����
	old_grandparent->right_child = node->left_child;
	node->left_child = old_grandparent;
	flush_height(old_grandparent);
	flush_bf(old_grandparent);

	flush_height(node);
	flush_bf(node);

}

template <typename K, typename E>
void AVLTree<K, E>::rotate_node(AVLNodePtr &parent)
{
	// ���Ӹ���
	if (parent->bf == 2)
	{
		// ��Ϊ���ӵ����ӵ��µ� ִ���ҵ���
		if (parent->left_child != nullptr && parent->left_child->bf == 1)
		{
			rotate_right(parent);
		}
		// ��Ϊ���ӵ��Һ��ӵ��µ� ִ���������˫��
		else if (parent->left_child != nullptr && parent->left_child->bf == -1)
		{
			rotate_left_right(parent);
		}
		// ��Ϊɶ ֱ������
		else if (parent->left_child != nullptr && parent->left_child->bf == 0)
		{
			rotate_right(parent);
		}
	}
	// �Һ��Ӹ���
	else if (parent->bf == -2)
	{
		// ��Ϊ�Һ��ӵ����ӵ��µ� ִ�����Һ���˫��
		if (parent->right_child != nullptr && parent->right_child->bf == 1)
		{
			rotate_right_left(parent);
		}
		// ��Ϊ�Һ��ӵ��Һ��ӵ��µ� ִ������
		else if (parent->right_child != nullptr && parent->right_child->bf == -1)
		{
			rotate_left(parent);
		}
		// ��Ϊɶ ֱ������
		else if (parent->right_child != nullptr && parent->right_child->bf == 0)
		{
			rotate_left(parent);
		}
	}
}


template<typename K, typename E>
const std::shared_ptr<std::pair<K, E>> AVLTree<K, E>::find(const K & key) const
{
	AVLNodePtr current = root;
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

template <typename K, typename E>
void AVLTree<K, E>::insert(const std::pair<K, E> & in_pair)
{
	AVLNodePtr current = root;
	AVLNodePtr parent = nullptr;
	std::stack<AVLNodePtr> parents;
	// �ҵ�������λ�� �Լ���Ӧ�ĸ��ڵ�
	while (current != nullptr)
	{
		parent = current;
		parents.push(current);
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
	// ִ�в������
	AVLNodePtr new_node = std::make_shared<AVLNode<K, E>>(in_pair);
	if (parents.empty())
	{
		root = new_node;
		return;
	}
	else
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
	// ���ϸ���
	current = new_node;
	while (!parents.empty())
	{
		parent = parents.top();
		parents.pop();
		flush_height(parent);
		flush_bf(parent);
		// ���º�ڵ��ƽ��ֵΪ0 �˳�
		if (parent->bf == 0)
		{
			break;
		}
		// ���º�ڵ��|bf| == 1 ˵�����ĸ߶ȱ���
		else if (parent->bf == 1 || parent->bf == -1)
		{
			current = parent;
		}
		// ���º�ڵ��|bf| == 2 ˵������ƽ��
		else
		{
			rotate_node(parent);
			break;
		}
	}
	// ���м�ϵ�������
	if (parents.empty())
	{
		root = parent;
	}
	else
	{
		AVLNodePtr parent_parent = parents.top();
		parents.pop();
		// ԭ���Ľڵ��λ��Ӧ���µĽڵ�������
		if (parent->element.first < parent_parent->element.first)
		{
			parent_parent->left_child = parent;
			flush_height(parent_parent);
		}
		else
		{
			parent_parent->right_child = parent;
			flush_height(parent_parent);
		}
		// ���ϸ������ĸ߶�
		while (!parents.empty())
		{
			flush_height(parents.top());
			parents.pop();
		}
	}

}

template <typename K, typename E>
void AVLTree<K, E>::earse(const K &key)
{
	AVLNodePtr current = root;
	AVLNodePtr parent = nullptr;
	std::stack<AVLNodePtr> parents;
	// �ҵ���ɾ��λ�� �Լ���Ӧ�ĸ��ڵ�
	while (current != nullptr && current->element.first != key)
	{
		parent = current;
		parents.push(parent);
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
		AVLNodePtr left_big = current->left_child;
		AVLNodePtr left_big_parent = current;
		parents.push(current);
		// �ҳ��������е����Ԫ��
		while (left_big->right_child != nullptr)
		{
			left_big_parent = left_big;	
			// Ҫ��¼ȥδ��ɾ���ڵ�·���ϵ����и��ڵ�
			parents.push(left_big_parent);
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
	}
	else
	{
		// ��ɾ���ڵ�������һ���ӽڵ�
		// ֱ������Ҫ�ֳ� ��ɾ���ڵ�û�к��� ��ɾ���ڵ�ֻ��һ�������������
		// ������ʵ�ǿ���ͳһ��
		AVLNodePtr temp = nullptr;
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
	// ���ϸ��¸��ڵ�ĸ߶Ⱥ�ƽ������
	// ��תǰ�������ڵ�  ��ת����������ڵ�
	AVLNodePtr old_parent = nullptr, new_parent = nullptr;
	while (!parents.empty())
	{
		parent = parents.top();
		parents.pop();
		// �����תǰ�������ڵ����丸�ڵ������ ��ô��ת����������ڵ��Ǹ��ڵ������
		// ��֮ ���Һ���
		if (parent != nullptr && parent->left_child == old_parent)
		{
			parent->left_child = new_parent;
		}
		else if (parent != nullptr && parent->right_child == old_parent)
		{
			parent->right_child = new_parent;
		}
		old_parent = parent;
		flush_height(parent);
		flush_bf(parent);
		rotate_node(parent);
		new_parent = parent;
	}
}

#endif //AVLTREE_AVLTREE_H

//
// Created by zhaossbnu on 2017/11/30.
//

#ifndef AVLTREE_REDBLACKTREE_H
#define AVLTREE_REDBLACKTREE_H

#include <iostream>
#include <utility>
#include <memory>
#include <vector>

#define RedBlackNodePtr std::shared_ptr<RedBlackNode<K, E>>
#define RED true
#define BLACK false

template <typename K, typename E>
struct RedBlackNode
{
	bool color;
	std::pair<K, E> element;
	RedBlackNodePtr left_child;
	RedBlackNodePtr right_child;
	RedBlackNodePtr parent;
	RedBlackNode(std::pair<K, E> data)
	{
		color = RED;
		element = data;
		left_child = right_child = nullptr;
		parent = nullptr;
	}
	friend std::ostream &operator<<(std::ostream &out, const RedBlackNode &node)
	{
		if (node.color)
		{
			out << "R" << "\t";
		}
		else
		{
			out << "B" << "\t";
		}
		out << node.element.first << "\t" << node.element.second;
		return out;
	}
};

template <typename K, typename E>
class RedBlackTree
{
public:
	RedBlackTree()
	{

	}
	RedBlackTree(const std::vector<std::pair<K, E>> &datas)
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
	RedBlackNodePtr root = nullptr;
	void in_order_out(const RedBlackNodePtr tree)
	{
		if (tree != nullptr)
		{
			in_order_out(tree->left_child);
			std::cout << *tree << std::endl;
			in_order_out(tree->right_child);
		}
	}
	// ����
	void rotate_left(RedBlackNodePtr &node);
	// �ҵ���
	void rotate_right(RedBlackNodePtr &node);
	// �������˫��
	void rotate_left_right(RedBlackNodePtr &node);
	// ���Һ���˫��
	void rotate_right_left(RedBlackNodePtr &node);
};


template <typename K, typename E>
void RedBlackTree<K, E>::rotate_left(RedBlackNodePtr &node)
{
	RedBlackNodePtr old_parent = node;
	node = node->right_child;
	if (old_parent->parent == nullptr)
	{
		node->parent = nullptr;
	}
	else
	{
		node->parent = old_parent->parent;
		if (old_parent->parent->left_child == old_parent)
		{
			old_parent->parent->left_child = node;
		}
		else
		{
			old_parent->parent->right_child = node;
		}
	}
	old_parent->right_child = node->left_child;
	if (node->left_child != nullptr)
	{
		node->left_child->parent = old_parent;
	}
	node->left_child = old_parent;
	old_parent->parent = node;
}

template <typename K, typename E>
void RedBlackTree<K, E>::rotate_right(RedBlackNodePtr &node)
{
	RedBlackNodePtr old_parent = node;
	node = node->left_child;
	if (old_parent->parent == nullptr)
	{
		node->parent = nullptr;
	}
	else
	{
		node->parent = old_parent->parent;
		if (old_parent->parent->left_child == old_parent)
		{
			old_parent->parent->left_child = node;
		}
		else
		{
			old_parent->parent->right_child = node;
		}
	}
	old_parent->left_child = node->right_child;
	if (node->right_child != nullptr)
	{
		node->right_child->parent = old_parent;
	}
	node->right_child = old_parent;
	old_parent->parent = node;
}

template <typename K, typename E>
void RedBlackTree<K, E>::rotate_left_right(RedBlackNodePtr &node)
{
	RedBlackNodePtr old_grandparent = node, old_parent = node->left_child;
	// ����
	node = old_parent->right_child;
	if (old_grandparent->parent == nullptr)
	{
		node->parent = nullptr;
	}
	else
	{
		node->parent = old_grandparent->parent;
		if (old_grandparent->parent->left_child == old_grandparent)
		{
			old_grandparent->parent->left_child = node;
		}
		else
		{
			old_grandparent->parent->right_child = node;
		}
	}
	old_parent->right_child = node->left_child;
	if (node->left_child != nullptr)
	{
		node->left_child->parent = old_parent;
	}
	node->left_child = old_parent;
	old_parent->parent = node;
	// ����
	old_grandparent->left_child = node->right_child;
	if (node->right_child != nullptr)
	{
		node->right_child->parent = old_grandparent;
	}
	node->right_child = old_grandparent;
	old_grandparent->parent = node;
}


template <typename K, typename E>
void RedBlackTree<K, E>::rotate_right_left(RedBlackNodePtr &node)
{
	RedBlackNodePtr old_grandparent = node, old_parent = node->right_child;
	// ����
	node = old_parent->left_child;
	if (old_grandparent->parent == nullptr)
	{
		node->parent = nullptr;
	}
	else
	{
		node->parent = old_grandparent->parent;
		if (old_grandparent->parent->left_child == old_grandparent)
		{
			old_grandparent->parent->left_child = node;
		}
		else
		{
			old_grandparent->parent->right_child = node;
		}
	}
	old_parent->left_child = node->right_child;
	if (node->right_child != nullptr)
	{
		node->right_child->parent = old_parent;
	}
	node->right_child = old_parent;
	old_parent->parent = node;
	// ����
	old_grandparent->right_child = node->left_child;
	if (node->left_child != nullptr)
	{
		node->left_child->parent = old_grandparent;
	}
	node->left_child = old_grandparent;
	old_grandparent->parent = node;
}

template<typename K, typename E>
const std::shared_ptr<std::pair<K, E>> RedBlackTree<K, E>::find(const K & key) const
{
	RedBlackNodePtr current = root;
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
			return std::make_shared(current->element);
		}
	}
	return nullptr;
}

template <typename K, typename E>
void RedBlackTree<K, E>::insert(const std::pair<K, E> & in_pair)
{
	RedBlackNodePtr current = root;
	RedBlackNodePtr parent = nullptr;
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
			current->element.second = in_pair.second;
			return;
		}
	}
	RedBlackNodePtr new_node = std::make_shared<RedBlackNode<K, E>>(in_pair);
	if (root == nullptr)
	{
		root = new_node;
		// ���ڵ����ɫһ���Ǻ�ɫ
		root->color = BLACK;
		return;
	}
	else
	{
		if (in_pair.first < parent->element.first)
		{
			parent->left_child = new_node;
			new_node->parent = parent;
		}
		else
		{
			parent->right_child = new_node;
			new_node->parent = parent;
		}
	}

	current = new_node;
	while (true)
	{
		RedBlackNodePtr parent = nullptr;
		RedBlackNodePtr grand_parent = nullptr;

		// �鿴��ǰ�ڵ�ĸ��ڵ�
		if (current->parent != nullptr)
		{
			parent = current->parent;
			if (parent->color == BLACK)
			{
				break;
			}
		}
		else
		{
			current->color = BLACK;
			root = current;
			break;
		}

		// �鿴��ǰ�ڵ���游�ڵ�
		if (parent->parent != nullptr)
		{
			grand_parent = parent->parent;
		}
		else
		{
			parent->color = BLACK;
			root = parent;
			break;
		}
		// ֻ�е�ǰ�ڵ�ʱ��ɫ ���ڵ��Ǻ�ɫ �游�ڵ��Ǻ�ɫ������²���Ҫ����
		// XXr�����ֻ��Ҫ������ɫ����
		// XXb�������Ҫ��ת����
		if (current->color == RED && parent->color == RED && grand_parent->color == BLACK)
		{
			// LL
			if (current == parent->left_child && parent == grand_parent->left_child)
			{
				// LLr
				if (grand_parent->right_child != nullptr && grand_parent->right_child->color == RED)
				{
					grand_parent->color = RED;
					grand_parent->right_child->color = BLACK;
					parent->color = BLACK;
					// Ϊ�´�ѭ����������������׼��
					current = grand_parent;
				}
				// LLb
				else if (grand_parent->right_child == nullptr || grand_parent->right_child->color == BLACK)
				{

					grand_parent->color = RED;
					parent->color = BLACK;
					rotate_right(grand_parent);

					current = parent;
				}
			}
			// LR
			else if (current == parent->right_child && parent == grand_parent->left_child)
			{
				// LRr
				if (grand_parent->right_child != nullptr && grand_parent->right_child->color == RED)
				{
					grand_parent->color = RED;
					grand_parent->right_child->color = BLACK;
					parent->color = BLACK;

					current = grand_parent;
				}
				// LRb
				else if (grand_parent->right_child == nullptr || grand_parent->right_child->color == BLACK)
				{
					grand_parent->color = RED;
					current->color = BLACK;
					rotate_left_right(grand_parent);
				}
			}
			// RL
			else if (current == parent->left_child && parent == grand_parent->right_child)
			{
				// RLr
				if (grand_parent->left_child != nullptr && grand_parent->left_child->color == RED)
				{
					grand_parent->color = RED;
					grand_parent->left_child->color = BLACK;
					parent->color = BLACK;

					current = grand_parent;
				}
				//RLb
				else if (grand_parent->left_child == nullptr || grand_parent->left_child->color == BLACK)
				{
					grand_parent->color = RED;
					current->color = BLACK;
					rotate_right_left(grand_parent);
				}
			}
			// RR
			else if (current == parent->right_child && parent == grand_parent->right_child)
			{
				// RRr
				if (grand_parent->left_child != nullptr && grand_parent->left_child->color == RED)
				{
					grand_parent->color = RED;
					grand_parent->left_child->color = BLACK;
					parent->color = BLACK;

					current = grand_parent;
				}
				//RRb
				else if (grand_parent->left_child == nullptr || grand_parent->left_child->color == BLACK)
				{
					parent->color = BLACK;
					grand_parent->color = RED;
					rotate_left(grand_parent);

					current = parent;
				}
			}
		}
		else
		{
			break;
		}
	}
}

template <typename K, typename E>
void RedBlackTree<K, E>::earse(const K &key)
{
	RedBlackNodePtr current = root;
	RedBlackNodePtr parent = nullptr;
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

	// �����滻�ڵ�
	RedBlackNodePtr replace_node = nullptr;
	if (current->left_child != nullptr)
	{
		replace_node = current->left_child;
		while (replace_node->right_child != nullptr)
		{
			replace_node = replace_node->right_child;
		}
	}
	else if (current->right_child != nullptr)
	{
		replace_node = current->right_child;
		while (replace_node->left_child != nullptr)
		{
			replace_node = replace_node->left_child;
		}
	}
	else
	{
		replace_node = current;
	}

	current->element = replace_node->element;
	//��current�ó�����ڵ� ����current������һ���ӽڵ�
	current = replace_node;
	parent = current->parent;
	// ��ǰ�ڵ�ʱ���ڵ�  ��ƽ��û��Ӱ��
	if (parent == nullptr)
	{
		root = nullptr;
		return;
	}
	// ��ǰ�ڵ���ɫΪ�� ��ƽ��û��Ӱ��
	if (current->color == RED)
	{
		if (current->left_child == nullptr && current->right_child == nullptr)
		{
			if (parent->left_child == current)
			{
				parent->left_child = nullptr;
			}
			else
			{
				current->parent->right_child = nullptr;
			}
		}
		else
		{
			if (current->left_child != nullptr)
			{
				current->left_child->parent = current->parent;
				if (current->parent->left_child == current)
				{
					current->parent->left_child = current->left_child;
				}
				else
				{
					current->parent->right_child = current->left_child;
				}
			}
			else
			{
				current->right_child->parent = current->parent;
				if (current->parent->left_child == current)
				{
					current->parent->left_child = current->right_child;
				}
				else
				{
					current->parent->right_child = current->right_child;
				}
			}
		}
		return;
	}

	// ��ǰ�ڵ���ɫΪ��ɫ ���Ҳ��Ǹ��ڵ� ��Ӱ�쵽����ƽ����
	// ����Ҫɾ����ǰ����ڽڵ�
	if (current->color == BLACK)
	{
		if (parent->left_child == current)
		{
			if (current->left_child != nullptr)
			{
				parent->left_child = current->left_child;
				current->left_child->parent = parent;
				current = current->left_child;
			}
			else if (current->right_child != nullptr)
			{
				parent->left_child = current->right_child;
				current->right_child->parent = parent;
				current = current->right_child;
			}
			else
			{
				parent->left_child = nullptr;
				current = nullptr;
			}
		}
		else
		{
			if (current->left_child != nullptr)
			{
				parent->right_child = current->left_child;
				current->left_child->parent = parent;
				current = current->left_child;
			}
			else if (current->right_child != nullptr)
			{
				parent->right_child = current->right_child;
				current->right_child->parent = parent;
				current = current->right_child;
			}
			else
			{
				parent->right_child = nullptr;
				current = nullptr;
			}
		}
	}

	// L/R ����ڵ������丸�ڵ�����ӻ����Һ���
	// b/r ����ڵ���ֵ��Ǻ�ɫ���Ǻ�ɫ
	// 0/1/2  ����b��ʱ �����ֵܽڵ��ɫ���ӵĸ���
	// 0/1/2  ����Rr��ʱ ���ֵܽڵ��Һ��ӵĺ�ɫ���Ӹ���
	// 0/1/2  ����Lr��ʱ ���ֵܽڵ����ӵĺ�ɫ���Ӹ���
	while (true)
	{
		RedBlackNodePtr brother = nullptr;
		// ��ǰ�ڵ㲻�Ǹոձ�ɾ���Ķ����Ǹ�ʱ
		if (current != nullptr && current->parent == nullptr)
		{
			current->color = BLACK;
			break;
		}
		// ��ǰ�ڵ㲻�Ǹոձ�ɾ�� ˵���Ǻ����Ĵ������ �丸�ڵ�Ҫ�ҵ�
		if (current != nullptr)
		{
			parent = current->parent;
		}
		// R
		if (parent->right_child == current)
		{
			brother = parent->left_child;
			//Rb
			if (brother->color == BLACK)
			{
				// Rb2
				if (brother->left_child != nullptr && brother->right_child != nullptr && brother->left_child->color == RED && brother->right_child->color == RED)
				{
					brother->right_child->color = parent->color;
					parent->color = BLACK;
					rotate_left_right(parent);
					break;
				}
				// Rb1(i)
				else if (brother->left_child != nullptr && brother->left_child->color == RED && (brother->right_child == nullptr || brother->right_child->color == BLACK))
				{
					brother->color = parent->color;
					brother->left_child->color = BLACK;
					parent->color = BLACK;

					rotate_right(parent);
					break;
				}
				// Rb1(ii)
				else if (brother->right_child != nullptr && brother->right_child->color == RED && (brother->left_child == nullptr || brother->left_child->color == BLACK))
				{
					brother->right_child->color = parent->color;
					parent->color = BLACK;
					rotate_left_right(parent);
					break;
				}
				// Rb0
				else
				{
					bool old_color = parent->color;
					brother->color = RED;
					parent->color = BLACK;
					if (old_color == RED)
					{
						break;
					}
					else
					{
						if (parent->parent == nullptr)
						{
							break;
						}
						else
						{
							current = parent;
							continue;
						}
					}
				}
			}
			//Rr
			else
			{
				RedBlackNodePtr brother_right = brother->right_child;
				//Rr2
				if (brother_right->left_child != nullptr && brother_right->right_child != nullptr && brother_right->left_child->color == RED && brother_right->right_child->color == RED)
				{
					RedBlackNodePtr new_parent = brother_right->right_child;
					new_parent->color = parent->color;

					brother_right->right_child = new_parent->left_child;
					if (new_parent->left_child != nullptr)
					{
						new_parent->left_child->parent = brother_right;
					}

					parent->left_child = new_parent->right_child;
					if (new_parent->right_child != nullptr)
					{
						new_parent->right_child->parent = parent;
					}

					new_parent->parent = parent->parent;
					if (parent->parent != nullptr && parent->parent->left_child == parent)
					{
						parent->parent->left_child = new_parent;
					}
					else if (parent->parent != nullptr && parent->parent->right_child == parent)
					{
						parent->parent->right_child = new_parent;
					}
					new_parent->left_child = brother;
					brother->parent = new_parent;
					new_parent->right_child = parent;
					parent->parent = new_parent;
					parent->color = BLACK;
					break;
				}
				// Rr1(i)
				else if (brother_right->left_child != nullptr && brother_right->left_child->color == RED && (brother_right->right_child == nullptr || brother_right->right_child->color == BLACK))
				{
					brother_right->left_child->color = BLACK;
					rotate_left_right(parent);
					break;
				}
				// Rr1(ii)
				else if (brother_right->right_child != nullptr && brother_right->right_child->color == RED && (brother_right->left_child == nullptr || brother_right->left_child->color == BLACK))
				{
					RedBlackNodePtr new_parent = brother_right->right_child;
					new_parent->color = parent->color;

					brother_right->right_child = new_parent->left_child;
					if (new_parent->left_child != nullptr)
					{
						new_parent->left_child->parent = brother;
					}

					parent->left_child = new_parent->right_child;
					if (new_parent->right_child != nullptr)
					{
						new_parent->right_child->parent = parent;
					}

					new_parent->parent = parent->parent;
					if (parent->parent != nullptr && parent->parent->left_child == parent)
					{
						parent->parent->left_child = new_parent;
					}
					else if (parent->parent != nullptr && parent->parent->right_child == parent)
					{
						parent->parent->right_child = new_parent;
					}
					new_parent->left_child = brother;
					brother->parent = new_parent;
					new_parent->right_child = parent;
					parent->parent = new_parent;
					parent->color = BLACK;
					break;
				}
				// Rr0
				else
				{
					brother->color = BLACK;
					brother->right_child->color = RED;
					rotate_right(parent);
					break;
				}
			}
		}
		// L
		else if (parent->left_child == current)
		{
			brother = parent->right_child;
			//Lb
			if (brother->color == BLACK)
			{
				// Lb2
				if (brother->left_child != nullptr && brother->right_child != nullptr && brother->left_child->color == RED && brother->right_child->color == RED)
				{
					brother->left_child->color = parent->color;
					parent->color = BLACK;
					rotate_right_left(parent);
					break;
				}
				// Lb1(i)
				else if (brother->right_child != nullptr && brother->right_child->color == RED && (brother->left_child == nullptr || brother->left_child->color == BLACK))
				{
					brother->right_child->color = BLACK;
					brother->color = parent->color;
					parent->color = BLACK;
					rotate_right(parent);
					break;
				}
				// Lb1(ii)
				else if (brother->left_child != nullptr && brother->left_child->color == RED && (brother->right_child == nullptr || brother->right_child->color == BLACK))
				{
					brother->left_child->color = parent->color;
					parent->color = BLACK;
					rotate_right_left(parent);
					break;
				}
				// Lb0
				else
				{
					bool old_color = parent->color;
					brother->color = RED;
					parent->color = BLACK;
					if (old_color == RED)
					{
						break;
					}
					else
					{
						if (parent->parent == nullptr)
						{
							break;
						}
						else
						{
							current = parent;
							continue;
						}
					}
				}
			}
			//Lr
			else
			{
				RedBlackNodePtr brother_left = brother->left_child;
				//Lr2
				if (brother_left->left_child != nullptr && brother_left->right_child != nullptr && brother_left->left_child->color == RED && brother_left->right_child->color == RED)
				{
					RedBlackNodePtr new_parent = brother_left->left_child;
					new_parent->color = parent->color;

					brother_left->left_child = new_parent->right_child;
					if (new_parent->right_child != nullptr)
					{
						new_parent->right_child->parent = brother_left;
					}

					parent->right_child = new_parent->left_child;
					if (new_parent->left_child != nullptr)
					{
						new_parent->left_child->parent = parent;
					}


					new_parent->parent = parent->parent;
					if (parent->parent != nullptr && parent->parent->left_child == parent)
					{
						parent->parent->left_child = new_parent;
					}
					else if (parent->parent != nullptr && parent->parent->right_child == parent)
					{
						parent->parent->right_child = new_parent;
					}
					new_parent->right_child = brother;
					brother->parent = new_parent;
					new_parent->left_child = parent;
					parent->parent = new_parent;
					break;
				}
				// Lr1(i)
				else if (brother_left->right_child != nullptr && brother_left->right_child->color == RED && (brother_left->left_child == nullptr || brother_left->left_child->color == BLACK))
				{
					brother_left->right_child->color = BLACK;
					rotate_left_right(parent);
					break;
				}
				// Lr1(ii)
				else if (brother_left->left_child != nullptr && brother_left->left_child->color == RED && (brother_left->right_child == nullptr || brother_left->right_child->color == BLACK))
				{
					RedBlackNodePtr new_parent = brother_left->left_child;
					new_parent->color = parent->color;

					brother_left->left_child = new_parent->right_child;
					if (new_parent->right_child != nullptr)
					{
						new_parent->right_child->parent = brother;
					}

					parent->right_child = new_parent->left_child;
					if (new_parent->left_child != nullptr)
					{
						new_parent->left_child->parent = parent;
					}

					new_parent->parent = parent->parent;
					if (parent->parent != nullptr && parent->parent->left_child == parent)
					{
						parent->parent->left_child = new_parent;
					}
					else if (parent->parent != nullptr && parent->parent->right_child == parent)
					{
						parent->parent->right_child = new_parent;
					}

					new_parent->right_child = brother;
					brother->parent = new_parent;
					new_parent->left_child = parent;
					parent->parent = new_parent;
					parent->color = BLACK;
					break;
				}
				// Rr0
				else
				{
					brother->color = BLACK;
					brother->right_child->color = RED;
					rotate_right(parent);
					break;
				}
			}
		}
	}
}

#endif //AVLTREE_REDBLACKTREE_H
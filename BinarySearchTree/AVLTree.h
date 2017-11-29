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
	// 节点平衡因子 是左子树的高度减去右子数
	int bf;
	// 树的高度 单个节点的高度为1
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
	AVLNodePtr root;
	// 中序遍历
	void in_order_out(const AVLNodePtr tree)
	{
		if (tree != nullptr)
		{
			in_order_out(tree->left_child);
			std::cout << *tree << std::endl;
			in_order_out(tree->right_child);
		}
	}
	// 获取某一个子树的高度
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
	// 刷新子树高度
	void flush_height(AVLNodePtr &node)
	{
		node->height = std::max(get_height(node->left_child), get_height(node->right_child)) + 1;
	}
	// 刷新子树平衡因子
	void flush_bf(AVLNodePtr &node)
	{
		node->bf = get_height(node->left_child) - get_height(node->right_child);
	}
	// 左单旋
	void rotate_left(AVLNodePtr &node);
	// 右单旋
	void rotate_right(AVLNodePtr &node);
	// 先左后右双旋
	void rotate_left_right(AVLNodePtr &node);
	// 先右后左双旋
	void rotate_right_left(AVLNodePtr &node);
	// 调整某一子树使其平衡
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
	// 左旋
	node = old_parent->right_child;
	old_parent->right_child = node->left_child;
	node->left_child = old_parent;
	flush_height(old_parent);
	flush_bf(old_parent);
	// 右旋
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
	// 右旋
	node = old_parent->left_child;
	old_parent->left_child = node->right_child;
	node->right_child = old_parent;
	flush_height(old_parent);
	flush_bf(old_parent);
	// 左旋
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
	// 左孩子高了
	if (parent->bf == 2)
	{
		// 因为左孩子的左孩子导致的 执行右单旋
		if (parent->left_child != nullptr && parent->left_child->bf == 1)
		{
			rotate_right(parent);
		}
		// 因为左孩子的右孩子导致的 执行先左后右双旋
		else if (parent->left_child != nullptr && parent->left_child->bf == -1)
		{
			rotate_left_right(parent);
		}
		// 不为啥 直接右旋
		else if (parent->left_child != nullptr && parent->left_child->bf == 0)
		{
			rotate_right(parent);
		}
	}
	// 右孩子高了
	else if (parent->bf == -2)
	{
		// 因为右孩子的左孩子导致的 执行先右后左双旋
		if (parent->right_child != nullptr && parent->right_child->bf == 1)
		{
			rotate_right_left(parent);
		}
		// 因为右孩子的右孩子导致的 执行左单旋
		else if (parent->right_child != nullptr && parent->right_child->bf == -1)
		{
			rotate_left(parent);
		}
		// 不为啥 直接右旋
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
	// 找到待插入位置 以及相应的父节点
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
			// 如果有重复关键字 则直接修改节点对应的值
			current->element.second = in_pair.second;
			return;
		}
	}
	// 执行插入操作
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
	// 向上更新
	current = new_node;
	while (!parents.empty())
	{
		parent = parents.top();
		parents.pop();
		flush_height(parent);
		flush_bf(parent);
		// 更新后节点的平衡值为0 退出
		if (parent->bf == 0)
		{
			break;
		}
		// 更新后节点的|bf| == 1 说明树的高度变了
		else if (parent->bf == 1 || parent->bf == -1)
		{
			current = parent;
		}
		// 更新后节点的|bf| == 2 说明树不平衡
		else
		{
			rotate_node(parent);
			break;
		}
	}
	// 把中间断的树接上
	if (parents.empty())
	{
		root = parent;
	}
	else
	{
		AVLNodePtr parent_parent = parents.top();
		parents.pop();
		// 原来的节点的位置应由新的节点来代替
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
		// 向上更新树的高度
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
	// 找到待删除位置 以及相应的父节点
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
	// 如果没有找到删除节点
	if (current == nullptr)
	{
		std::cout << "No element whose key is " << key << std::endl;
		return;
	}

	// 待删除节点有两个孩子
	if (current->left_child != nullptr && current->right_child != nullptr)
	{
		AVLNodePtr left_big = current->left_child;
		AVLNodePtr left_big_parent = current;
		parents.push(current);
		// 找出左子树中的最大元素
		while (left_big->right_child != nullptr)
		{
			left_big_parent = left_big;	
			// 要记录去未来删除节点路径上的所有父节点
			parents.push(left_big_parent);
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
	}
	else
	{
		// 待删除节点至多有一个子节点
		// 直观上需要分成 待删除节点没有孩子 待删除节点只有一个孩子两种情况
		// 但是其实是可以统一的
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
	// 向上更新各节点的高度和平衡因子
	// 旋转前子树根节点  旋转后的子树根节点
	AVLNodePtr old_parent = nullptr, new_parent = nullptr;
	while (!parents.empty())
	{
		parent = parents.top();
		parents.pop();
		// 如果旋转前子树根节点是其父节点的左孩子 那么旋转后的子树根节点是父节点的左孩子
		// 反之 是右孩子
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

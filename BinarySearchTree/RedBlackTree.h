//
// Created by zhaossbnu on 2017/11/30.
//

#ifndef AVLTREE_REDBLACKTREE_H
#define AVLTREE_REDBLACKTREE_H

#include <iostream>
#include <utility>
#include <memory>
#include <vector>
#include <stack>
#include <algorithm>

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
            ascend();
            std::cout << std::endl;
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
};

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
        // 根节点的颜色一定是黑色
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

        // 查看当前节点的父节点
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

        // 查看当前节点的祖父节点
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
        // 只有当前节点时红色 父节点是红色 祖父节点是黑色的情况下才需要调整
        // XXr的情况只需要调整颜色即可
        // XXb的情况需要旋转子树
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
                    // 为下次循环继续向上设置做准备
                    current = grand_parent;
                }
                    // LLb
                else if (grand_parent->right_child == nullptr || grand_parent->right_child->color == BLACK)
                {
                    if (grand_parent->parent != nullptr)
                    {
                        if (grand_parent->parent->left_child == grand_parent)
                        {
                            grand_parent->parent->left_child = parent;
                        }
                        else if (grand_parent->parent->right_child = grand_parent)
                        {
                            grand_parent->parent->right_child = parent;
                        }
                    }
                    parent->parent = grand_parent->parent;
                    parent->color = BLACK;
                    grand_parent->parent = parent;
                    grand_parent->left_child = parent->right_child;
                    grand_parent->color = RED;
                    parent->right_child = grand_parent;

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
                    parent->right_child = current->left_child;
                    if (current->left_child != nullptr)
                    {
                        current->left_child->parent = parent;
                    }

                    grand_parent->left_child = current->right_child;
                    if (current->right_child != nullptr)
                    {
                        current->right_child->parent = grand_parent;
                    }
                    if (grand_parent->parent != nullptr)
                    {
                        if (grand_parent->parent->left_child == grand_parent)
                        {
                            grand_parent->parent->left_child = current;
                        }
                        else if (grand_parent->parent->right_child = grand_parent)
                        {
                            grand_parent->parent->right_child = current;
                        }
                    }
                    current->parent = grand_parent->parent;
                    current->left_child = parent;
                    parent->parent = current;
                    current->right_child = grand_parent;
                    grand_parent->parent = current;
                    current->color = BLACK;
                    grand_parent->color = RED;
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
                    parent->left_child = current->right_child;
                    if (current->right_child != nullptr)
                    {
                        current->right_child->parent = parent;
                    }

                    grand_parent->right_child = current->left_child;
                    if (current->left_child != nullptr)
                    {
                        current->left_child->parent = grand_parent;
                    }
                    if (grand_parent->parent != nullptr)
                    {
                        if (grand_parent->parent->left_child == grand_parent)
                        {
                            grand_parent->parent->left_child = current;
                        }
                        else if (grand_parent->parent->right_child = grand_parent)
                        {
                            grand_parent->parent->right_child = current;
                        }
                    }
                    current->parent = grand_parent->parent;
                    current->right_child = parent;
                    parent->parent = current;
                    current->left_child = grand_parent;
                    grand_parent->parent = current;
                    current->color = BLACK;
                    grand_parent->color = RED;
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
                    if (grand_parent->parent != nullptr)
                    {
                        if (grand_parent->parent->left_child == grand_parent)
                        {
                            grand_parent->parent->left_child = parent;
                        }
                        else if (grand_parent->parent->right_child = grand_parent)
                        {
                            grand_parent->parent->right_child = parent;
                        }
                    }
                    parent->parent = grand_parent->parent;
                    parent->color = BLACK;
                    grand_parent->right_child = parent->left_child;
                    parent->left_child->parent = grand_parent;
                    parent->left_child = grand_parent;
                    grand_parent->parent = parent;
                    grand_parent->color = RED;

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

}

#endif //AVLTREE_REDBLACKTREE_H

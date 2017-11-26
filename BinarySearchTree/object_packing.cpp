#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "DuplicateBinarySearchTree.h"
using namespace std;

int main()
{
    vector<pair<int, string>> v =
            {
                    {1, "a"},
                    {3, "b"},
                    {12, "c"},
                    {6, "d"},
                    {8, "e"},
                    {1, "f"},
                    {20, "g"},
                    {6, "h"},
                    {5, "i"}
            };
    DuplicateBinarySearchTree<int, string> tree(v);
    vector<int> objects = {4, 5, 7, 8, 1, 30};
    pair<int, string> update;
    for(vector<int>::size_type i = 0; i < objects.size(); ++i)
    {
        shared_ptr<pair<int, string>> best = tree.findGE(objects[i]);
        if(best == nullptr)
        {
            cout << "Can't find box!!!" << endl;
        }
        else
        {
            update = *best;
            // 删除原来的箱子
            tree.earse(best->first);
            cout << "object " << i + 1 << " in box " << update.second << endl;
            update.first -= objects[i];
            // 箱子不为空 将箱子插入树中
            if(update.first)
            {
                tree.insert(update);
            }
        }
    }
    return 0;
}
#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <iostream>
#include <vector>

// Definition for a binary tree node.
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

inline TreeNode *MakeTree(const std::vector<int> &data, int index)
{
    TreeNode *root = nullptr;
    if (index < data.size() && data[index] != 0) {
        root = new TreeNode(data[index]);
        root->left = MakeTree(data, 2 * index + 1);
        root->right = MakeTree(data, 2 * index + 2);
    }
    return root;
}

inline void PrintTreeNode(TreeNode *root, std::vector<int> &data, int type)
{
    if (root == nullptr) {
        return;
    }
    // 前序遍历
    if (type == 0) {
        data.push_back(root->val);
    }
    PrintTreeNode(root->left, data, type);
    // 中序遍历
    if (type == 1) {
        data.push_back(root->val);
    }
    PrintTreeNode(root->right, data, type);
    // 后序遍历
    if (type == 2) {
        data.push_back(root->val);
    }
}

#endif  // TREE_NODE_H
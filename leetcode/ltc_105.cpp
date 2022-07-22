#include "action.h"
#include <algorithm>

#include "tree_node.h"

using namespace std;

class Solution {
public:
    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
    {
        if (preorder.size() == 0 || inorder.size() == 0)
            return nullptr;

        int val = preorder[0];
        auto iter = find(inorder.begin(), inorder.end(), val);
        int leftSize = distance(inorder.begin(), iter);

        vector<int> preorderLeft(preorder.begin() + 1, preorder.begin() + 1 + leftSize);
        vector<int> inorderLeft(inorder.begin(), inorder.begin() + leftSize);

        vector<int> preorderRight(preorder.begin() + 1 + leftSize, preorder.end());
        vector<int> inorderRight(inorder.begin() + leftSize + 1, inorder.end());

        TreeNode *root = new TreeNode(val);
        root->left = buildTree(preorderLeft, inorderLeft);
        root->right = buildTree(preorderRight, inorderRight);
        return root;
    }

    TreeNode *buildTree2(vector<int> &inorder, vector<int> &postorder)
    {
        if (inorder.size() == 0 || postorder.size() == 0)
            return nullptr;

        int val = postorder[postorder.size() - 1];
        auto iter = find(inorder.begin(), inorder.end(), val);
        int leftSize = distance(inorder.begin(), iter);

        vector<int> inorderLeft(inorder.begin(), inorder.begin() + leftSize);
        vector<int> postorderLeft(postorder.begin(), postorder.begin() + leftSize);

        vector<int> inorderRight(inorder.begin() + 1 + leftSize, inorder.end());
        vector<int> postorderRight(postorder.begin() + leftSize, postorder.end() - 1);

        TreeNode *root = new TreeNode(val);
        root->left = buildTree(inorderLeft, postorderLeft);
        root->right = buildTree(inorderRight, postorderRight);
        return root;
    }
};

void test_tree_105_106()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<int> preorder = {3, 9, 20, 15, 7};   // 前序遍历
    vector<int> inorder = {9, 3, 15, 20, 7};    // 中序遍历
    vector<int> postorder = {9, 15, 7, 20, 3};  // 后序遍历

    Solution su;
    // TreeNode *root = su.buildTree(preorder, inorder);
    TreeNode *root = su.buildTree2(inorder, postorder);

    vector<int> data = {};
    PrintTreeNode(root, data, 0);
    for (const auto &iter : data) {
        cout << iter << " ";
    }
    cout << endl;
}
LTC_REGISTER_ACTION(ACTION_LTC, test_tree_105_106);
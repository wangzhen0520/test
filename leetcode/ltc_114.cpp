#include "action.h"
#include <algorithm>

#include "tree_node.h"

using namespace std;

class Solution {
public:
    void flatten(TreeNode *root)
    {
        if (root == nullptr) {
            return;
        }

        flatten(root->left);
    
        TreeNode *right = root->right;
        root->right = root->left;
        root->left = nullptr;

        while (root->right != nullptr) {
            root = root->right;
        }

        flatten(right);
        root->right = right;
    }
    void flatten2(TreeNode *root)
    {
        if (root == nullptr) {
            return;
        }

        flatten(root->left);
        flatten(root->right);
    
        // TreeNode *left = root->left;
        TreeNode *right = root->right;
        root->right = root->left;
        root->left = nullptr;

        while (root->right != nullptr) {
            root = root->right;
        }        
        root->right = right;
    }
};

void test_tree_114()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<int> nums = {1, 2, 5, 3, 4, NULL, 6};
    int index = 0;
    TreeNode *root = MakeTree(nums, index);

    Solution su;
    su.flatten2(root);

    vector<int> data = {};
    PrintTreeNode(root, data, 0);
    for (const auto &iter : data) {
        cout << iter << " ";
    }
    cout << endl;
}
LTC_REGISTER_ACTION(ACTION_LTC, test_tree_114);
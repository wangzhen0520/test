#include "action.h"
#include <algorithm>

#include "tree_node.h"

using namespace std;

class Solution {
public:
    int maxDepth(TreeNode *root)
    {
        if (root == nullptr) {
            return 0;
        }

        int leftDep = maxDepth(root->left);
        int rightDep = maxDepth(root->right);

        // 整棵树的最大深度等于左右子树的最大深度取最大值，
        // 然后再加上根节点自己
        int res = max(leftDep, rightDep) + 1;
        return res;
    }
};

void test_tree_104()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<int> nums = {3, 9, 20, NULL, NULL, 15, 7};
    int index = 0;
    TreeNode *root = MakeTree(nums, index);

    Solution su;
    int ret = su.maxDepth(root);
    cout << ret << endl;

    vector<int> data = {};
    PrintTreeNode(root, data, 0);
    for (const auto &iter : data) {
        cout << iter << " ";
    }
    cout << endl;
    data.clear();

    PrintTreeNode(root, data, 1);
    for (const auto &iter : data) {
        cout << iter << " ";
    }
    cout << endl;
    data.clear();

    PrintTreeNode(root, data, 2);
    for (const auto &iter : data) {
        cout << iter << " ";
    }
    cout << endl;
    data.clear();
}
LTC_REGISTER_ACTION(ACTION_LTC, test_tree_104);
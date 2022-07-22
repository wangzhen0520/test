#include "action.h"
#include <algorithm>

#include "tree_node.h"

using namespace std;

class Solution {
public:
    TreeNode *constructMaximumBinaryTree(vector<int> &nums)
    {
        if (!nums.size()){
			return nullptr;
		}
        auto p = max_element(nums.begin(), nums.end());
        vector<int> left(nums.begin(), p);
        vector<int> right(p + 1, nums.end());

        TreeNode *root = new TreeNode(*p);
        root->left = constructMaximumBinaryTree(left);
        root->right = constructMaximumBinaryTree(right);
        return root;
    }
};

void test_tree_654()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<int> nums = {3, 2, 1, 6, 0, 5};

    Solution su;
    TreeNode *root = su.constructMaximumBinaryTree(nums);

    vector<int> data = {};
    PrintTreeNode(root, data, 0);
    for (const auto &iter : data) {
        cout << iter << " ";
    }
    cout << endl;
}
LTC_REGISTER_ACTION(ACTION_LTC, test_tree_654);
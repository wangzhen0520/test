/*
你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统，
如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警。

给定一个代表每个房屋存放金额的非负整数数组，计算你 不触动警报装置的情况下 ，一夜之内能够偷窃到的最高金额。

 

示例 1：

输入：[1,2,3,1]
输出：4
解释：偷窃 1 号房屋 (金额 = 1) ，然后偷窃 3 号房屋 (金额 = 3)。
     偷窃到的最高金额 = 1 + 3 = 4 。
示例 2：

输入：[2,7,9,3,1]
输出：12
解释：偷窃 1 号房屋 (金额 = 2), 偷窃 3 号房屋 (金额 = 9)，接着偷窃 5 号房屋 (金额 = 1)。
     偷窃到的最高金额 = 2 + 9 + 1 = 12 。

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/house-robber
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include "action.h"
#include <algorithm>

using namespace std;

int steps = 0;
void printIndent(int n) {
    for (int i = 0; i < n; i++) {
        cout << "   ";
    }
}

class Solution {
public:
    int rob(vector<int>& nums) {
        memo.assign(nums.size(), -1);
        return dp(nums, 0);
    }

    int dp(vector<int>& nums, int s) 
    {
        printIndent(steps++);
        cout << s << endl;
        if (s >= nums.size()) {
            printIndent(--steps);
            cout << "return 0" << endl;
            return 0;
        }

        if (memo[s] != -1) {
            printIndent(--steps);
            cout << "return memo[" << s << "]: " << memo[s] << endl;
            return memo[s];
        }

        int res = max(dp(nums, s + 1), nums[s] + dp(nums, s + 2));

        memo[s] = res;

        printIndent(--steps);
        cout << "return " << res << endl;
        return res;
    }

private:
    vector<int> memo;
};

void test_tree_198()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<int> nums = {2, 1, 1, 2};
    // vector<int> nums = {1, 2, 3, 1};

    Solution su;
    int ret = su.rob(nums);

    cout << ret << endl;
}
LTC_REGISTER_ACTION(ACTION_LTC, test_tree_198);
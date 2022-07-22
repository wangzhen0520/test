/*
给定一个非负整数数组 nums ，你最初位于数组的 第一个下标 。

数组中的每个元素代表你在该位置可以跳跃的最大长度。

判断你是否能够到达最后一个下标。

 

示例 1：

输入：nums = [2,3,1,1,4]
输出：true
解释：可以先跳 1 步，从下标 0 到达下标 1, 然后再从下标 1 跳 3 步到达最后一个下标。
示例 2：

输入：nums = [3,2,1,0,4]
输出：false
解释：无论怎样，总会到达下标为 3 的位置。但该下标的最大跳跃长度是 0 ， 所以永远不可能到达最后一个下标。

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/jump-game
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include "action.h"
#include <algorithm>

using namespace std;

class Solution {
public:
    // 贪心算法
    bool canJump(vector<int> &nums)
    {
        int n = nums.size();

        int fa = 0;
        for (int i = 0; i < n - 1; i++) {
            fa = max(fa, i + nums[i]);
            if (fa < i) {
                return false;
            }
        }

        return fa >= n - 1;
    }

    // 动态规划
    bool canJump2(vector<int> &nums)
    {
        // dp[i] 表示i的位置最大可以跳到的位置
        int n = nums.size();
        vector<int> dp(n);

        dp[0] = nums[0];
        for (int i = 1; i < n; i++) {
            if (dp[i - 1] < i) {
                return false;
            }
            dp[i] = max(dp[i - 1], i + nums[i]);

            if (dp[i] >= n - 1) {
                return true;
            }
        }

        return true;
    }
};

void test_tree_55()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    // vector<int> nums = {2, 3, 1, 1, 4};
    vector<int> nums = {1, 2};

    Solution su;
    bool ret = su.canJump2(nums);

    cout << (ret ? "true" : "false") << endl;
}
LTC_REGISTER_ACTION(ACTION_LTC, test_tree_55);
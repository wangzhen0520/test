/*
给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

 

示例 1：



输入：height = [0,1,0,2,1,0,1,3,2,1,2,1]
输出：6
解释：上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 
示例 2：

输入：height = [4,2,0,3,2,5]
输出：9

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/trapping-rain-water
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include "action.h"

using namespace std;

class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        vector<int> lftMax(n);
        vector<int> rhtMax(n);

        lftMax[0] = height[0];
        rhtMax[n - 1] = height[n - 1];
        for (int i = 1; i < n; i++) {
            lftMax[i] = max(lftMax[i - 1], height[i]);
        }
        
        for (int i = n-2; i >= 0; i--) {
            rhtMax[i] = max(rhtMax[i + 1], height[i]);
        }

        int res = 0;
        for (int i = 1; i < n - 1; i++) {
            res += min(lftMax[i], rhtMax[i]) - height[i];
        }
        
        return res;
    }
};

void test_trap()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<int> nums = {0,1,0,2,1,0,1,3,2,1,2,1};

    Solution su;
    int ret = su.trap(nums);

    cout << ret << endl;
}
LTC_REGISTER_ACTION(ACTION_LTC, test_trap);
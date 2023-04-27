/*
给定一个包含非负整数的数组 nums ，返回其中可以组成三角形三条边的三元组个数。

 

示例 1:

输入: nums = [2,2,3,4]
输出: 3
解释:有效的组合是:
2,3,4 (使用第一个 2)
2,3,4 (使用第二个 2)
2,2,3
示例 2:

输入: nums = [4,2,3,4]
输出: 4
 

提示:

1 <= nums.length <= 1000
0 <= nums[i] <= 1000

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/valid-triangle-number
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include "action.h"
#include <algorithm>

using namespace std;

class Solution {
public:
    int triangleNumber(vector<int> &nums)
    {
        int n = nums.size();
        sort(nums.begin(), nums.end());
        int ans = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                // for (int k = j - 1; k >= 0; k--) {
                //     if (nums[j] + nums[k] > nums[i]) {
                //         ans++;
                //     }
                // }
                int left = j + 1;
                int right = n - 1;
                int k = j;

                while (left <= right) {
                    int mid = (right - left) / 2 + left;

                    if (nums[i] + nums[j] > nums[mid]) {
                        k = mid;
                        left = mid + 1;
                    } else {
                        right = mid - 1;
                    }
                }

                ans += k - j;
            }
        }

        return ans;
    }
};

void test_ltc_611(int argc, char *argv[])
{
#define test
#ifdef test
    return;
#else
    cout << __FILE__ << ":" << __func__ << endl;
#endif

    vector<int> nums = {2, 2, 3, 4};

    Solution su;
    auto ret = su.triangleNumber(nums);

    cout << ret << endl;
}

LTC_REGISTER_ACTION(ACTION_LTC, test_ltc_611);
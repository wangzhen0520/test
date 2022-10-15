/*
给你一个整数数组 nums ，判断是否存在三元组 [nums[i], nums[j], nums[k]] 满足 i != j、i != k 且 j != k ，同时还满足 nums[i] + nums[j] + nums[k] == 0 。请
你返回所有和为 0 且不重复的三元组。
注意：答案中不可以包含重复的三元组。
示例 1：

输入：nums = [-1,0,1,2,-1,-4]
输出：[[-1,-1,2],[-1,0,1]]
解释：
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0 。
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0 。
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0 。
不同的三元组是 [-1,0,1] 和 [-1,-1,2] 。
注意，输出的顺序和三元组的顺序并不重要。
示例 2：

输入：nums = [0,1,1]
输出：[]
解释：唯一可能的三元组和不为 0 。
示例 3：

输入：nums = [0,0,0]
输出：[[0,0,0]]
解释：唯一可能的三元组和为 0 。
 

提示：

3 <= nums.length <= 3000
-105 <= nums[i] <= 105

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/3sum
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include "action.h"
#include <algorithm>

using namespace std;

class Solution1 {
public:
    struct ThreeNums
    {
        int num1_;
        int num2_;
        int num3_;

        ThreeNums(int num1, int num2, int num3):num1_(num1),num2_(num2),num3_(num3) {
        }
    };
    struct cmp {
        bool operator() (const ThreeNums& a, const ThreeNums& b) const {
            if (a.num1_ == b.num1_) {
                if (a.num2_ == b.num2_) {
                    return a.num3_ < b.num3_;
                } else {
                    return a.num2_ < b.num2_;
                }
            } else {
                return a.num1_ < b.num1_;
            }
        }
    };

    vector<vector<int>> threeSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> ret = {};
        int count = nums.size();
        for (size_t i = 0; i < count; i++) {
            int num1 = nums[i];
            if (num1 > 0 || (i > 0 && nums[i] == nums[i - 1])) {
                continue;
            }
            for (size_t j = i + 1; j < count; j++) {
                int num2 = nums[j];
                int left = 0 - (num1 + num2);
                for (size_t k = j + 1; k < count; k++) {
                    if (nums[k] > left) {
                        break;
                    } else if (nums[k] == left) {
                        // ThreeNums tmp(num1, num2, nums[k]);
                        // ret_map[tmp]++;
                        vector<int> tmp;
                        tmp.push_back(num1);
                        tmp.push_back(num2);
                        tmp.push_back(nums[k]);
                        ret.push_back(tmp);
                    }
                }
            }
        }
        return ret;
    }
};

class Solution
{
public:
    vector<vector<int>> twoSum(vector<int>& nums, int start, int end, int target, int value)
    {
        vector<vector<int>> ans = {};
        while (start < end) {
            int sum = nums[start] + nums[end];
            if (sum == target) {
                vector<int> ret;
                ret.push_back(value);
                ret.push_back(nums[start]);
                ret.push_back(nums[end]);
                ans.push_back(ret);

                while (start < end && nums[start] == nums[start + 1]) { // 去重
                    start++;
                }
                start++;
                while (start < end && nums[end] == nums[end - 1]) { // 去重
                    end--;
                }
                end--;
            } else if (sum < target) {
                start++;
            } else {
                end--;
            }
        }
        return ans;
    }

    vector<vector<int>> threeSum(vector<int>& nums)
    {
        vector<vector<int>> ans = {};
        sort(nums.begin(), nums.end());
        int count = nums.size();
        for (size_t i = 0; i < count; i++) {
            if (i > 0 && nums[i] == nums[i - 1]) { // 去重
                continue;
            }
            auto ret = twoSum(nums, i + 1, count - 1, -nums[i], nums[i]);
            ans.insert(ans.end(), ret.begin(), ret.end());
        }

        return ans;
    }
};

void test_ltc_15()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<int> nums = {-1,0,1,2,-1,-4};
    Solution su;
    TimeCost tc;
    auto ret = su.threeSum(nums);
    // tc.OperateCost("threeSum");
    for (size_t i = 0; i < ret.size(); i++) {
        for (size_t j = 0; j < ret[i].size(); j++) {
            cout << ret[i][j] << " ";
        }
        cout << endl;
        if (i > 10) {
            break;
        }
    }
}

LTC_REGISTER_ACTION(ACTION_LTC, test_ltc_15);
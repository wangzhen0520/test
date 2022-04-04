/*
34. 在排序数组中查找元素的第一个和最后一个位置
给定一个按照升序排列的整数数组 nums，和一个目标值 target。找出给定目标值在数组中的开始位置和结束位置。

如果数组中不存在目标值 target，返回 [-1, -1]。

进阶：

你可以设计并实现时间复杂度为 O(log n) 的算法解决此问题吗？


示例 1：

输入：nums = [5,7,7,8,8,10], target = 8
输出：[3,4]
示例 2：

输入：nums = [5,7,7,8,8,10], target = 6
输出：[-1,-1]
示例 3：

输入：nums = [], target = 0
输出：[-1,-1]


提示：

0 <= nums.length <= 105
-109 <= nums[i] <= 109
nums 是一个非递减数组
-109 <= target <= 109
*/
#include <algorithm>
#include <iostream>
#include <iterator>
#include <ostream>
#include <vector>

using namespace std;

int binary_search(vector<int> &nums, int target)
{
    int left = 0, right = nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        } else if (nums[mid] == target) {
            // 直接返回
            return mid;
        }
    }
    // 直接返回
    return -1;
}

// 寻找左侧边界的二分查找
int left_bound(vector<int> &nums, int target)
{
    int left = 0;
    int right = nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        } else if (nums[mid] == target) {
            // 别返回，锁定左侧边界
            right = mid - 1;
        }
    }
    // 最后要检查 left 越界的情况
    if (left >= nums.size() || nums[left] != target) {
        return -1;
    }
    return left;
}

// 寻找右侧边界的二分查找
int right_bound(vector<int> &nums, int target)
{
    int left = 0;
    int right = nums.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        } else if (nums[mid] == target) {
            // 别返回，锁定右侧边界
            left = mid + 1;
        }
    }
    // 最后要检查 right 越界的情况
    if (right < 0 || nums[right] != target) {
        return -1;
    }
    return right;
}

class Solution {
public:
    vector<int> searchRange(vector<int> &nums, int target)
    {
        // int n = nums.size();
        // vector<int> ret = {};
        // for (int i = 0; i < n; i++) {
        //     if (nums[i] == target) {
        //         if (ret.empty()) {
        //             ret.push_back(i);
        //         }
        //         else {
        //             if (ret.size() > 2) {
        //                 ret.pop_back();
        //             }

        //             ret.push_back(i);
        //         }
        //     }
        // }

        // if (ret.empty()) {
        //     ret = {-1, -1};
        // }
        // else if (ret.size() == 1) {
        //     ret = {ret[0], ret[0]};
        // }
        int left = left_bound(nums, target);
        int right = right_bound(nums, target);

        return {left, right};
    }

    int search(vector<int> &nums, int target)
    {
        // return binary_search(nums, target);

        int n = nums.size();

        int split = 0;
        for (int i = 0; i < n; i++) {
            if (nums[0] < nums[i]) {
                split++;
            }
        }

        cout << "split: " << split << endl;

        int left = split + 1;
        int right = n - 1;
        if (nums[0] <= target && nums[split] >= target) {
            left = 0;
            right = split;
        }

        cout << "left: " << left << endl;
        cout << "right: " << right << endl;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] < target) {
                left = mid + 1;
            } else if (nums[mid] > target) {
                right = mid - 1;
            } else if (nums[mid] == target) {
                // 直接返回
                return mid;
            }
        }
        // 直接返回
        return -1;
    }
};

void test_tree_34()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif
    Solution su;

    // vector<int> nums = {5,7,7,8,8,10};
    // int target = 8;
    // vector<int> ret = su.searchRange(nums, target);

    // for (const auto &iter : ret) {
    //     cout << iter << endl;
    // }

    // 旋转数组查找
    vector<int> nums = {4, 5, 6, 7, 0, 1, 2};
    int target = 0;
    int ret = su.search(nums, target);
    cout << ret << endl;
}
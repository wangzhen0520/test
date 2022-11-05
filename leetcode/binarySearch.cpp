#include "binarySearch.h"

#include <iostream>
#include <vector>

using namespace std;

/*
二分搜索问题的泛化
什么问题可以运用二分搜索算法技巧？
首先，你要从题目中抽象出一个自变量 x，一个关于 x 的函数 f(x)，以及一个目标值 target。
同时，x, f(x), target 还要满足以下条件：
1、f(x) 必须是在 x 上的单调函数（单调增单调减都可以）。
2、题目是让你计算满足约束条件 f(x) == target 时的 x 的值。

// 函数 f 是关于自变量 x 的单调函数
int f(int x) {
    // ...
}

// 主函数，在 f(x) == target 的约束下求 x 的最值
int solution(int[] nums, int target) {
    if (nums.length == 0) return -1;
    // 问自己：自变量 x 的最小值是多少？
    int left = ...;
    // 问自己：自变量 x 的最大值是多少？
    int right = ... + 1;

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (f(mid) == target) {
            // 问自己：题目是求左边界还是右边界？
            // ...
        } else if (f(mid) < target) {
            // 问自己：怎么让 f(x) 大一点？
            // ...
        } else if (f(mid) > target) {
            // 问自己：怎么让 f(x) 小一点？
            // ...
        }
    }
    return left;
}

// 二分查找框架
int binarySearch(int nums[], int target)
{
    int left = 0, right = ...;

    while (...) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            ...;
        } else if (nums[mid] < target) {
            left = ...;
        } else if (nums[mid] > target) {
            right = ...;
        }
    }
    return ...;
}
*/

int binarySearch(vector<int> &nums, int target)
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
            // 直接返回
            return mid;
        }
    }

    return -1;
}

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
            // 锁定左边界
            right = mid - 1;
        }
    }

    if (left >= nums.size() || nums[left] != target) {
        return -1;
    }

    return left;
}

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
            // 锁定右边界
            left = mid + 1;
        }
    }

    if (right < 0 || nums[right] != target) {
        return -1;
    }

    return right;
}

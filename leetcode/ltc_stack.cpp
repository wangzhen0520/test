#include <iostream>
#include <vector>
#include <stack>

using namespace std;

// 单调栈

/*
给定一个循环数组（最后一个元素的下一个元素是数组的第一个元素），输出每个元素的下一个更大元素。
数字 x 的下一个更大的元素是按数组遍历顺序，
这个数字之后的第一个比它更大的数，这意味着你应该循环地搜索它的下一个更大的数。如果不存在，则输出 -1。

示例 1:

输入: [1,2,1]
输出: [2,-1,2]
解释: 第一个 1 的下一个更大的数是 2；
数字 2 找不到下一个更大的数； 
第二个 1 的下一个最大的数需要循环搜索，结果也是 2。

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/next-greater-element-ii
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

/*
vector<int> nextGreaterElement(vector<int>& nums) {
    vector<int> res(nums.size()); // 存放答案的数组
    stack<int> s;
    // 倒着往栈里放
    for (int i = nums.size() - 1; i >= 0; i--) {
        // 判定个子高矮
        while (!s.empty() && s.top() <= nums[i]) {
            // 矮个起开，反正也被挡着了。。。
            s.pop();
        }
        // nums[i] 身后的 next great number
        res[i] = s.empty() ? -1 : s.top();
        s.push(nums[i]);
    }
    return res;
}
*/

class Solution {
public:
    vector<int> nextGreaterElements(vector<int>& nums) {
        int n = nums.size();
        vector<int> res(n);
        stack<int> s;
        // 假装这个数组长度翻倍了
        for (int i = 2 * n - 1; i >= 0; i--) {
            // 索引要求模，其他的和模板一样
            while (!s.empty() && s.top() <= nums[i % n])
                s.pop();
            res[i % n] = s.empty() ? -1 : s.top();
            s.push(nums[i % n]);
        }
        return res;
    }
};

void test_ltc_stack()
{
#define test
#ifdef test
    return;
#else
    cout << __FILE__ << ":" << __func__ << endl;
#endif

    vector<int> nums = {2,1,2,4,3};

    Solution su;
    vector<int> res = su.nextGreaterElements(nums);

    for (int i = 0; i < res.size(); i++) {
        cout << res[i] << " ";
    }
    cout << endl;
}

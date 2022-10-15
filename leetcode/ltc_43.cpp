/*
给定两个以字符串形式表示的非负整数 num1 和 num2，返回 num1 和 num2 的乘积，它们的乘积也表示为字符串形式。

注意：不能使用任何内置的 BigInteger 库或直接将输入转换为整数。

 

示例 1:

输入: num1 = "2", num2 = "3"
输出: "6"
示例 2:

输入: num1 = "123", num2 = "456"
输出: "56088"
 

提示：

1 <= num1.length, num2.length <= 200
num1 和 num2 只能由数字组成。
num1 和 num2 都不包含任何前导零，除了数字0本身。

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/multiply-strings
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include "action.h"
#include <algorithm>

using namespace std;

class Solution {
public:
    string multiply(string num1, string num2)
    {
        int m = num1.size(), n = num2.size();
        // 结果最多为 m + n 位数
        vector<int> res(m + n, 0);
        // 从个位数开始逐位相乘
        for (int i = m - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                int mul = (num1[i] - '0') * (num2[j] - '0');
                // 乘积在 res 对应的索引位置
                int p1 = i + j;
                int p2 = i + j + 1;
                // 叠加到 res 上
                int sum = mul + res[p2];
                res[p2] = sum % 10;
                res[p1] += sum / 10;
            }
        }
        // 结果前缀可能存的 0（未使用的位）
        int i = 0;
        while (i < res.size() && res[i] == 0) {
            i++;
        }
        // 将计算结果转化成字符串
        string str;
        for (; i < res.size(); i++) {
            str.push_back('0' + res[i]);
        }

        return str.size() == 0 ? "0" : str;
    }
};

void test_ltc_43()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif
    TimeCost tc;

    string str1 = "123";
    string str2 = "45";

    Solution su;
    auto ret = su.multiply(str1, str2);
    cout << ret << endl;
}

LTC_REGISTER_ACTION(ACTION_LTC, test_ltc_43);
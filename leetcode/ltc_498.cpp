/*
给你一个大小为 m x n 的矩阵 mat ，请以对角线遍历的顺序，用一个数组返回这个矩阵中的所有元素。

 

示例 1：


输入：mat = [[1,2,3],[4,5,6],[7,8,9]]
输出：[1,2,4,7,5,3,6,8,9]
示例 2：

输入：mat = [[1,2],[3,4]]
输出：[1,2,3,4]

来源：力扣（LeetCode）
链接：https://leetcode.cn/problems/diagonal-traverse
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include "action.h"

using namespace std;

class Solution {
public:
    vector<int> findDiagonalOrder(vector<vector<int>>& mat) {
        int m = mat.size();
        int n = mat[0].size();
        vector<int> ret = {};
        int count = m + n - 1;

        for (int i = 0; i < count; i++) {
            if (i % 2) {
                int x = i < n ? 0 : i - n + 1;
                int y = i < n ? i : n - 1;
                while (x < m && y >= 0) {
                    ret.push_back(mat[x][y]);
                    x++;
                    y--;
                }
            } else {
                int x = i < m ? i : m - 1;
                int y = i < m ? 0 : i - m + 1;
                while (x >= 0 && y < n) {
                    ret.push_back(mat[x][y]);
                    x--;
                    y++;
                }
            }
        }

        return ret;
    }
};

void test_ltc_498()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<vector<int>> mat = {{1,2,3}, {4,5,6}, {7,8,9}};

    Solution su;
    auto ret = su.findDiagonalOrder(mat);
    for (const auto &iter : ret) {
        cout << iter << endl;
    }
}
LTC_REGISTER_ACTION(ACTION_LTC, test_ltc_498);
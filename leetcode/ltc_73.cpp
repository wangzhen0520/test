/*
给定一个 m x n 的矩阵，如果一个元素为 0 ，则将其所在行和列的所有元素都设为 0 。请使用 原地 算法。

 

示例 1：


输入：matrix = [[1,1,1],[1,0,1],[1,1,1]]
输出：[[1,0,1],[0,0,0],[1,0,1]]
示例 2：


输入：matrix = [[0,1,2,0],[3,4,5,2],[1,3,1,5]]
输出：[[0,0,0,0],[0,4,5,0],[0,3,1,0]]
 

提示：

m == matrix.length
n == matrix[0].length
1 <= m, n <= 200
-231 <= matrix[i][j] <= 231 - 1

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/set-matrix-zeroes
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include <algorithm>
#include <iostream>
#include <ostream>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    struct pos {
        int x;
        int y;
    };
    void setZeroes(vector<vector<int>> &matrix)
    {
        int m = matrix.size();
        int n = matrix[0].size();

        vector<pos> res;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == 0) {
                    cout << i << ", " << j << endl;
                    res.push_back({i, j});
                }
            }
        }

        for (auto &iter : res) {
            for (int i = 0; i < m; i++) {
                matrix[i][iter.y] = 0;
            }

            for (int j = 0; j < n; j++) {
                matrix[iter.x][j] = 0;
            }
        }
    }
};

void test_tree_73()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<vector<int>> nums = {{0, 1, 2, 0}, {3, 4, 5, 2}, {1, 3, 1, 5}};

    Solution su;
    su.setZeroes(nums);

    for (auto &iter : nums) {
        for (auto &iter2 : iter) {
            cout << iter2 << " ";
        }
        cout << endl;
    }
}
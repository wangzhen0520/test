/*
编写一个高效的算法来判断 m x n 矩阵中，是否存在一个目标值。该矩阵具有如下特性：

每行中的整数从左到右按升序排列。
每行的第一个整数大于前一行的最后一个整数。
 

示例 1：


输入：matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 3
输出：true
示例 2：


输入：matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 13
输出：false
 

提示：

m == matrix.length
n == matrix[i].length
1 <= m, n <= 100
-104 <= matrix[i][j], target <= 104

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/search-a-2d-matrix
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include "action.h"
#include <algorithm>

using namespace std;

class Solution {
public:
    bool searchMatrix(vector<vector<int>> &matrix, int target)
    {
        int m = matrix.size();
        int n = matrix[0].size();

        bool find = false;
        bool ret = true;
        int last = -100000;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == target) {
                    // cout << i << ", " << j << endl;
                    find = true;
                }

                if (matrix[i][j] < last) {
                    ret = false;
                    break;
                }

                last = matrix[i][j];
            }
        }

        return ret & find;
    }
};

void test_tree_74(int argc, char *argv[])
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    // vector<vector<int>> matrix = {{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};
    // int target = 3;
    vector<vector<int>> matrix = {{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};
    int target = 13;

    Solution su;
    bool ret = su.searchMatrix(matrix, target);

    cout << (ret ? "true" : "false") << endl;
}
LTC_REGISTER_ACTION(ACTION_LTC, test_tree_74);
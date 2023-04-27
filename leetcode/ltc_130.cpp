/*
给你一个 m x n 的矩阵 board ，由若干字符 'X' 和 'O' ，找到所有被 'X'
围绕的区域，并将这些区域里所有的 'O' 用 'X' 填充。

示例 1：

输入：board =
[["X","X","X","X"},{"X","O","O","X"},{"X","X","O","X"},{"X","O","X","X"]]
输出：[["X","X","X","X"},{"X","X","X","X"},{"X","X","X","X"},{"X","O","X","X"]]
解释：被围绕的区间不会存在于边界上，换句话说，任何边界上的 'O' 都不会被填充为 'X'。
任何不在边界上，
或不与边界上的 'O' 相连的 'O' 最终都会被填充为 'X'。如果两个元素在水平或垂直方向相邻，则称它们是“相连”的。
示例 2：

输入：board = [["X"]]
输出：[["X"]]

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/surrounded-regions
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include "action.h"
#include <algorithm>

#include "union_find.h"

using namespace std;

class Solution {
public:
    void solve(vector<vector<char>>& board)
    {
        int m = board.size();
        if (m == 0) {
            return;
        }

        int n = board[0].size();
        int any = m * n;

        UF uf(any + 1);

        for (int i = 0; i < m; i++) {
            if (board[i][0] == 'O') {
                uf.uion(i * n, any);
            }
            if (board[i][n - 1] == 'O') {
                uf.uion(i * n + n - 1, any);
            }
        }

        for (int i = 0; i < n; i++) {
            if (board[0][i] == 'O') {
                uf.uion(i * m, any);
            }
            if (board[m - 1][i] == 'O') {
                uf.uion(n * (m - 1) + i, any);
            }
        }

        vector<vector<int>> d = {{1, 0}, {0, 1}, {0, -1}, {-1, 0}};
        for (int i = 1; i < m - 1; i++) {
            for (int j = 1; j < n - 1; j++) {
                if (board[i][j] == 'O') {
                    for (int k = 0; k < 4; k++) {
                        int x = i + d[k][0];
                        int y = j + d[k][1];
                        if (board[x][y] == 'O') {
                            uf.uion(x * n + y, i * n + j);
                        }
                    }
                }
            }
        }

        for (int i = 1; i < m - 1; i++) {
            for (int j = 1; j < n - 1; j++) {
                if (!uf.connected(any, i * n + j)) {
                    board[i][j] = 'X';
                }
            }
        }
    }

    void show(vector<vector<char>>& board)
    {
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[i].size(); j++) {
                cout << board[i][j] << ',';
            }
            cout << endl;
        }
        cout << endl;
    }
};

void test_uf1(int argc, char *argv[])
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<vector<char>> nums = {{'X', 'X', 'X', 'X'},
                                 {'X', 'O', 'O', 'X'},
                                 {'X', 'X', 'O', 'X'},
                                 {'X', 'O', 'X', 'X'}};

    int ret = 0;
    Solution su;
    su.show(nums);
    su.solve(nums);
    su.show(nums);
}
LTC_REGISTER_ACTION(ACTION_LTC, test_uf1);
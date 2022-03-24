/*
给定一个 m x n 二维字符网格 board 和一个字符串单词 word 。如果 word 存在于网格中，返回 true ；否则，返回 false 。

单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。同一个单元格内的字母不允许被重复使用。

 

示例 1：


输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCCED"
输出：true
示例 2：


输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "SEE"
输出：true
示例 3：


输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCB"
输出：false
 

提示：

m == board.length
n = board[i].length
1 <= m, n <= 6
1 <= word.length <= 15
board 和 word 仅由大小写英文字母组成

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/word-search
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include <algorithm>
#include <iostream>
#include <iterator>
#include <ostream>
#include <vector>

using namespace std;

class Solution {
public:
    bool exist(vector<vector<char>> &board, string word) {
        int m = board.size();
        int n = board[0].size();

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                // cout << i << " " <<  j << " " << board[i][j] << endl;
                if (dfs(board, word, 0, i, j)) {
                    return true;
                }
            }
        }

        return false;
    }

    vector<pair<int, int>> directions{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    bool dfs(vector<vector<char>> &board, string &word, int u, int x, int y) {
        if (board[x][y] != word[u]) {
            return false;
        }
        if (u == word.size() - 1) {
            return true;
        }

        char t = board[x][y];
        board[x][y] = '.';
        for (const auto& iter: directions) {
            int a = x + iter.first;
            int b = y + iter.second;
            if (a < 0 || a >= board.size() || b < 0 || b >= board[0].size() || board[a][b] == '.') {
                continue;
            }
            if (dfs(board, word, u + 1, a, b)) {
                return true;
            }
        }
        board[x][y] = t;
        return false;
    }
};

void test_tree_79()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    vector<vector<char>> board = {{'A', 'B', 'C', 'E'}, {'S', 'F', 'C', 'S'}, {'A', 'D', 'E', 'E'}};
    string word = "ABCCED";

    Solution su;
    bool ret = su.exist(board, word);

    cout << (ret ? "true" : "false") << endl;
}
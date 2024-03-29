/*
给定一个由表示变量之间关系的字符串方程组成的数组，每个字符串方程 equations[i] 的长度为 4，
并采用两种不同的形式之一："a==b" 或 "a!=b"。在这里，a 和 b 是小写字母（不一定不同），表示单字母变量名。

只有当可以将整数分配给变量名，以便满足所有给定的方程时才返回 true，否则返回 false。 

示例 1：

输入：["a==b","b!=a"]
输出：false
解释：如果我们指定，a = 1 且 b = 1，那么可以满足第一个方程，但无法满足第二个方程。没有办法分配变量同时满足这两个方程。
示例 2：

输入：["b==a","a==b"]
输出：true
解释：我们可以指定 a = 1 且 b = 1 以满足满足这两个方程。
示例 3：

输入：["a==b","b==c","a==c"]
输出：true
示例 4：

输入：["a==b","b!=c","c==a"]
输出：false
示例 5：

输入：["c==c","b==d","x!=z"]
输出：true

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/satisfiability-of-equality-equations
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
#include "action.h"

#include "union_find.h"

using namespace std;

class Solution {
public:
    bool equationsPossible(vector<string>& equations)
    {
        UF uf(26);

        for (const auto &iter : equations) {
            if (iter.at(1) == '=') {
                uf.uion(iter.at(0) - 'a', iter.at(3) - 'a');
            }
        }

        for (const auto &iter : equations) {
            if ((iter.at(1) == '!') && uf.connected(iter.at(0) - 'a', iter.at(3) - 'a')) {
                return false;
            }
        }
        return true;
    }
};

void test_uf2(int argc, char *argv[])
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    // vector<string> equations = {"a==b", "b!=a"};
    vector<string> equations = {"c==c","b==d","x!=z"};

    Solution su;
    cout << (su.equationsPossible(equations) ? "true" : "false") << endl;
}
LTC_REGISTER_ACTION(ACTION_LTC, test_uf2);
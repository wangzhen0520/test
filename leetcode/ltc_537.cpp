#include "action.h"
#include <sec_api/stdlib_s.h>
#include <stdlib.h>
#include <string>

using namespace std;

class Solution {
public:
    string complexNumberMultiply(string num1, string num2)
    {
        auto pos1 = num1.find('+');
        auto pos2 = num1.find('i');

        // cout << pos1 << "  " << pos2 << endl;

        int a = 0;
        if (pos1 != num1.npos) {
            a = stoi(num1.substr(0, pos1));
        }
        int b = 0;
        if (pos2 != num1.npos) {
            b = stoi(num1.substr(pos1 + 1, pos2));
        }
        // cout << a << endl;
        // cout << b << endl;

        pos1 = num2.find('+');
        pos2 = num2.find('i');

        int c = 0;
        if (pos1 != num2.npos) {
            c = stoi(num2.substr(0, pos1));
        }
        int d = 0;
        if (pos2 != num2.npos) {
            d = stoi(num2.substr(pos1 + 1, pos2));
        }
        // cout << c << endl;
        // cout << d << endl;

        return to_string(a * c - b * d) + "+" + to_string(a * d + b * c) + "i";
    }
};

void test_tree_537()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    string num1 = "1+-1i";
    string num2 = "0+0i";

    Solution su;
    string ret = su.complexNumberMultiply(num1, num2);

    cout << ret << endl;
}
LTC_REGISTER_ACTION(ACTION_LTC, test_tree_537);
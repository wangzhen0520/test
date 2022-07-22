#include "action.h"
#include <stack>

using namespace std;

bool isValid(string s)
{
    stack<char> str;

    for (char& i : s) {
        if (i == '{' || i == '[' || i == '(') {
            str.push(i);
        }
        else if (i == '}' || i == ']' || i == ')') {
            if (!str.empty() &&
                ((i == '}' && str.top() == '{') || (i == ']' && str.top() == '[') || (i == ')' && str.top() == '('))) {
                str.pop();
            }
            else {
                return false;
            }
        }
    }
    return str.empty();
}

void test_isvalid()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    string s;
    while (cin >> s) {
        cout << (isValid(s) ? "true" : "false") << endl;
    }
}
LTC_REGISTER_ACTION(ACTION_LTC, test_isvalid);
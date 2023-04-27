#include "action.h"

using namespace std;

int lengthOfLongestSubstring(string s)
{
    vector<int> m(256, -1);
    int left = -1;
    int res = 0;
    int len = s.size();
    for (int i = 0; i < len; i++) {
        left = max(left, m[s[i]]);
        m[s[i]] = i;
        res = max(res, i - left);
    }
    return res;

    /*
        int len = s.length();

        if (len < 2) return len;
        int m = 1, p = 0;
        for (int i = 1; i < len; ++i) {
            for (int k = p; k < i; ++k) {
                if (s[k] == s[i]) {
                    m = m > (i - p) ? m : (i - p);
                    p = k + 1;
                }
            }
        }
        m = m > (len - p) ? m : (len - p);
        return m;
    */
}

void test_substring(int argc, char *argv[])
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    string s = "abcabcbb";
    cout << lengthOfLongestSubstring(s) << endl;

    s = "ab";
    cout << lengthOfLongestSubstring(s) << endl;
}
LTC_REGISTER_ACTION(ACTION_LTC, test_substring);
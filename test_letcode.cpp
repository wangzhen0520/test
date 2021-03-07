#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>

#include "autoshare.h"
#include "test_letcode.h"

using namespace std;

vector<int> twoSum(vector<int>& nums, int target)
{
#if 0
    int len  = nums.size();
    vector<int> ret;

    for (int i = 0; i < len; ++i) {
        int a1 = nums[i];
        int b1 = target - a1;

        for (int j = i + 1; j < len; ++j) {
            if (nums[j] == b1) {
                //cout << i << "  " << j << endl;
                ret.push_back(i);
                ret.push_back(j);
                return {i, j};
            }
        }
    }
    return {};
#endif

    unordered_map<int, int> m;
    for (int i = 0; i < nums.size(); i++) {
        if (m.find(target - nums[i]) != m.end()) {
            return { m.find(target - nums[i])->second, i };
        } else
            m[nums[i]] = i;
    }
    return {};
}

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x)
        : val(x)
        , next(NULL)
    {
    }
};

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
{
    ListNode* head = new ListNode(0);
    ListNode* p1 = l1;
    ListNode* p2 = l2;
    ListNode* tmp = head;
    while (p1 && p2) {
        if (p1->val < p2->val) {
            tmp->next = p1;
            p1 = p1->next;
        } else {
            tmp->next = p2;
            p2 = p2->next;
        }
        tmp = tmp->next;
    }
    if (p1 == NULL) {
        tmp->next = p2;
    } else {
        tmp->next = p1;
    }
    return head->next;
}

ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
{
    ListNode* r = l1;
    ListNode* p1 = l1;
    ListNode* p2 = l2;
    int len1 = 0, len2 = 0;
    while (p1 != NULL || p2 != NULL) {
        int add1 = 0;
        int add2 = 0;
        if (p1 != NULL) {
            add1 = p1->val;

            len1++;
        }

        if (p2 != NULL) {
            add2 = p2->val;

            len2++;
        }

        r->val = add1 + add2;

        cout << "xxx: " << r->val << endl;

        if (len1 < len2) {
            r->next = p2->next;
            // len1 = len2;
        }

        p1 = p1 ? p1->next : NULL;
        p2 = p2 ? p2->next : NULL;

        if (len1 == len2) {
            r = p1 ? p1 : p2;
        }
    }

    ListNode* p = l1;
    while (p != NULL) {
        cout << "xxxx: " << p->val << endl;
        if (p->val >= 10) {
            int tmp = p->val;
            p->val = tmp % 10;
            if (p->next != NULL) {
                p->next->val += tmp / 10;
            } else {
                p->next = new ListNode(tmp / 10);
            }
        }
        p = p->next;
    }

    return l1;
}

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

    /*int len = s.length();

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
  return m;*/
}

int toint(string in)
{
    int rst;
    stringstream ss;
    ss << in;
    ss >> rst;
    return rst;
}

int priority(char a)
{
    switch (a) {
    case '*':
        return 2;
    case '/':
        return 2;
    case '+':
        return 1;
    case '-':
        return 1;
    case '(':
        return 3;
    case ')':
        return 3;
    }

    return -1;
}

bool isdig(char a)
{
    if (a >= '0' && a <= '9')
        return true;
    else
        return false;
}

//保证每次入栈的符号的优先级都比当前的栈顶元素要高，若此时栈顶的优先级比入栈元素低或者等于的话，则需要出栈
//知道遇到比当前需要入栈元素优先级高的为止
void midtopost(string in, vector<string>& vec)
{
    stack<char> s;
    string rst = "";
    int i = 0;
    while (true) {
        if (i >= in.length())
            break;
        if (isdig(in[i])) {
            string num = "";
            while (isdig(in[i])) {
                num += in[i++];
            }
            vec.push_back(num);
        } else {
            if (s.empty()) {
                s.push(in[i++]);
            } else {
                if (in[i] == '(') {
                    s.push(in[i]);
                } else if (in[i] == ')') {
                    while (s.top() != '(') {
                        string temp = "";
                        temp += s.top();
                        vec.push_back(temp);
                        s.pop();
                    }
                    s.pop();
                } else {
                    if (priority(in[i]) > priority(s.top()) || s.top() == '(') {
                        s.push(in[i]);
                    } else {
                        //判断是否为空必须写在前面，符合短路原则
                        while (!s.empty() && (priority(in[i]) <= priority(s.top()))) {
                            string temp = "";
                            temp += s.top();
                            vec.push_back(temp);
                            s.pop();
                        }
                        s.push(in[i]);
                    }
                }
                ++i;
            }
        }
    }
    //清空栈
    while (!s.empty()) {
        string temp = "";
        temp += s.top();
        vec.push_back(temp);
        s.pop();
    }
}

//后缀表达式的计算，数字进栈，符号将栈顶两个元素出栈，运算后进栈
int calc(vector<string>& vec)
{
    stack<int> s;
    for (int i = 0; i < vec.size(); ++i) {
        if (!vec[i].compare("*")) {
            int x = s.top();
            s.pop();
            int y = s.top();
            s.pop();
            s.push(x * y);
        } else if (!vec[i].compare("-")) {
            int x = s.top();
            s.pop();
            int y = s.top();
            s.pop();
            s.push(y - x);
        } else if (!vec[i].compare("+")) {
            int x = s.top();
            s.pop();
            int y = s.top();
            s.pop();
            s.push(x + y);
        } else if (!vec[i].compare("/")) {
            int x = s.top();
            s.pop();
            int y = s.top();
            s.pop();
            s.push(y / x);
        } else {
            s.push(toint(vec[i]));
        }
    }
    return s.top();
}

bool isValid(string s)
{
    stack<char> str;

    for (char& i : s) {
        if (i == '{' || i == '[' || i == '(') {
            str.push(i);
        } else if (i == '}' || i == ']' || i == ')') {
            if (!str.empty() && ((i == '}' && str.top() == '{') || (i == ']' && str.top() == '[') || (i == ')' && str.top() == '('))) {
                str.pop();
            } else {
                return false;
            }
        }
    }
    return str.empty();
}

int romanToInt(string s)
{
    int res = 0;
    for (int i = 0; i < s.size(); ++i) {
        char sn = s[i + 1];

        if (s[i] == 'I' && (sn == 'V' || sn == 'X')) {
            res += (sn == 'V') ? 4 : 9;
            ++i;
            continue;
        } else if (s[i] == 'X' && (sn == 'L' || sn == 'C')) {
            res += (sn == 'L') ? 40 : 90;
            ++i;
            continue;
        } else if (s[i] == 'C' && (sn == 'D' || sn == 'M')) {
            res += (sn == 'D') ? 400 : 900;
            ++i;
            continue;
        } else if (s[i] == 'I') {
            res += 1;
        } else if (s[i] == 'V') {
            res += 5;
        } else if (s[i] == 'X') {
            res += 10;
        } else if (s[i] == 'L') {
            res += 50;
        } else if (s[i] == 'C') {
            res += 100;
        } else if (s[i] == 'D') {
            res += 500;
        } else if (s[i] == 'M') {
            res += 1000;
        }
    }
    return res;
}

bool isBadVersion(int version) { return version >= 4; }

int firstBadVersion(int n)
{
    int left = 0;
    int right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (isBadVersion(right + 1)) {
            if (isBadVersion(mid)) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } else if (isBadVersion(left)) {
            if (isBadVersion(mid)) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    return left;
}

int f(int x, int& n)
{
    cout << x << " " << n++ << endl;
    if (x <= 2) {
        return 1;
    } else {
        return f(x - 2, n) + f(x - 4, n) + 1;
    }

    cout << "done" << endl;
}

int test_letcode(int argc, char* argv[])
{
    std::cout << "test_letcode" << std::endl;

#if 0
    int n;
    cin >> n;
    cout << firstBadVersion(n) << endl;
#endif

#if 0
    string haystack = "hello";
    string needle = "lo";

    int pos = -1;
    if ((pos = haystack.find(needle)) != string::npos) {
        cout << pos << endl;
    } else {
        cout << pos << endl;
    }

#endif

#if 0
    string s;
    while (cin >> s) {
        cout << (isValid(s) ? "true" : "false") << endl;
    }
#endif

#if 0
    string s;
    while (cin >> s) {
        cout << romanToInt(s) << endl;
    }
#endif

#if 0
    int x;

    while(cin >> x) {
        string s;
        stringstream sx;
        sx << x;
        sx >> s;

        string str = s;
        reverse(s.begin(), s.end());
        //cout << s << endl;
        if (str.compare(s) == 0) {
            cout << "true" << endl;
        } else {
            cout << "false" << endl;
        }
    }
#endif

#if 0
    string in="9+(3-1)*3+10/2";
    //string s="9 3 1 - 3 * + 10 2 / +";
    vector<string> vec;
    midtopost(in,vec);
    cout << calc(vec) << endl;
#endif

#if 0
    string s = "abcabcbb";

    cout << lengthOfLongestSubstring(s) << endl;

    s = "ab";
    cout << lengthOfLongestSubstring(s) << endl;

#endif

#if 0
    ListNode ls11(1);
    ListNode ls12(2);
    ListNode ls13(4);

    ListNode ls21(1);
    ListNode ls22(3);
    ListNode ls23(4);
    //ListNode ls24(1);

    ListNode *l;
    ListNode *l1;
    ListNode *l2;

    l1 = &ls11;
    ls11.next = &ls12;
    ls12.next = &ls13;

    l2 = &ls21;
    ls21.next = &ls22;
    ls22.next = &ls23;
    //ls23.next = &ls24;

    //l = addTwoNumbers(l1, l2);
    l = mergeTwoLists(l1, l2);

    ListNode *p = l;
    while (p != NULL) {
       cout << p->val << endl;
       p = p->next;
    }
#endif

#if 0
    int a[] = {2, 7, 11, 15};
    vector<int> nums(a, a+sizeof(a));
    int target = 9;

    vector<int> ret;
    ret = twoSum(nums, target);

    for (auto iter : ret) {
        cout << iter << endl;
    }
#endif

#if 0
#define N 3
//#define Y(n) ((N+1)*n)
#define Y(a, b) ((a + b) * a)

    //char acbuf3[5] = {'H', 'e', 'l'};
    //printf("%d", sizeof(acbuf3));

    //int z = 2*(N+Y(5+1));
    //int z = 2 * Y(3, 4+1);
    int a = 7;
    float x=2.5, y=4.7;

    printf("z: %f\n", x+a%3*(int)(x+y)%2/4);

    int i;
    char acNew[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (i = 0; i < 10; ++i) {
        acNew[i] = '0';
    }
    printf("%d\n", strlen(acNew));

    char cA;
    unsigned char ucB;
    unsigned short usC;
    cA=128;
    ucB=128;
    usC=cA+ucB;
    printf("0x%x ", usC);
    usC=cA+(short)ucB;
    printf("0x%x ", usC);
    usC = (unsigned char)cA+ucB;
    printf("0x%x ", usC);
    usC=cA+(char)ucB;
    printf("0x%x\n", usC);

    char a1,b1;
    a1 = 128;
    b1 = a1+1;
    printf("%d,%d\n", a1, b1);

#define MAX 255
    unsigned char A[MAX], k;
    for (k = 1; k < MAX; k++) {
        A[k] = k;
        if (k == 254)
        {
            printf("A[k]: %d\n", k, A[k]);
        }
    }
#endif

#if 0
    autoshared(argc, argv);
#endif

#if 0
    int n = 0;
    int a = f(6, n);
    cout << a << " "  << n << endl;
#endif
    return 0;
}
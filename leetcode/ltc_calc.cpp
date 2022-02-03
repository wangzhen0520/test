#include <iostream>
#include <sstream>
#include <stack>
#include <vector>


using namespace std;

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
        case '*': return 2;
        case '/': return 2;
        case '+': return 1;
        case '-': return 1;
        case '(': return 3;
        case ')': return 3;
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
        }
        else {
            if (s.empty()) {
                s.push(in[i++]);
            }
            else {
                if (in[i] == '(') {
                    s.push(in[i]);
                }
                else if (in[i] == ')') {
                    while (s.top() != '(') {
                        string temp = "";
                        temp += s.top();
                        vec.push_back(temp);
                        s.pop();
                    }
                    s.pop();
                }
                else {
                    if (priority(in[i]) > priority(s.top()) || s.top() == '(') {
                        s.push(in[i]);
                    }
                    else {
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
        }
        else if (!vec[i].compare("-")) {
            int x = s.top();
            s.pop();
            int y = s.top();
            s.pop();
            s.push(y - x);
        }
        else if (!vec[i].compare("+")) {
            int x = s.top();
            s.pop();
            int y = s.top();
            s.pop();
            s.push(x + y);
        }
        else if (!vec[i].compare("/")) {
            int x = s.top();
            s.pop();
            int y = s.top();
            s.pop();
            s.push(y / x);
        }
        else {
            s.push(toint(vec[i]));
        }
    }
    return s.top();
}

void test_calc()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    string in = "9+(3-1)*3+10/2";
    // string s="9 3 1 - 3 * + 10 2 / +";
    vector<string> vec;
    midtopost(in, vec);
    cout << calc(vec) << endl;
}
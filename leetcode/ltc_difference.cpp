#include <iostream>

#include "difference.h"

using namespace std;

void test_ltc_diff()
{
#define test
#ifdef test
    return;
#else
    cout << __FILE__ << ":" << __func__ << endl;
#endif

    vector<vector<int> > updates = {{1, 3, 2}, {2, 4, 3}, {0, 2, -2}};

    vector<int> nums(5);
    Difference df(nums);

    for (auto &iter : updates) {
        // cout << iter[0] << " " << iter[1] << " " << iter[2] << endl;
        df.increment(iter[0], iter[1], iter[2]);
    }
    // cout << endl;

    for (auto &iter : df.result()) {
        cout << iter << " ";
    }
    cout << endl;
}

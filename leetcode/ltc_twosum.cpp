#include "action.h"
#include <unordered_map>

using namespace std;

class Solution {
public:
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
};

void test_twosum()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    int a[] = {2, 7, 11, 15};
    vector<int> nums(a, a+sizeof(a));
    int target = 9;

    vector<int> ret;

    Solution su;
    ret = su.twoSum(nums, target);

    for (auto iter : ret) {
        cout << iter << endl;
    }
}
LTC_REGISTER_ACTION(ACTION_LTC, test_twosum);
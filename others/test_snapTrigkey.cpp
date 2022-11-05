#include <algorithm>

#include "action.h"

using namespace std;

using SnapTrigKey = struct SnapTrigKey {
    uint32_t begin;  // 抓拍计数开始
    uint32_t end;    // 抓拍计数结束, 正常情况下begin < end, 当begin是从254开始的时候 end 有能小于 begin

    bool operator<(const SnapTrigKey &key) const
    {
        if (begin == key.begin) {
            return end < key.end;
        } else {
            return begin < key.begin;
        }
    }

    bool operator==(const SnapTrigKey &key) const
    {
        // 满足在区间内
        // [254, 255] -> 254, 255
        if (key.begin <= key.end) {
            if (key.begin <= begin && begin <= key.end) {
                return true;
            }
        } else {
            // [254, 1] -> 254, 255, 1
            // [255, 2] -> 255, 1, 2
            if ((begin <= 255 && begin >= key.begin) || (begin >= 1 && begin <= key.end)) {
                return true;
            }
        }

        return false;
    }
};

void test_snapTrigkey()
{
#define test
#ifdef test
    return;
#else
    cout << __FILE__ << ":" << __func__ << endl;
#endif

    std::map<SnapTrigKey, int> tmp;

    // [2, 2]
    SnapTrigKey trigkey = {1, 2};
    tmp.emplace(std::make_pair(trigkey, 0));

    // [3, 4]
    trigkey.begin = 3;
    trigkey.end = 4;
    tmp.emplace(std::make_pair(trigkey, 1));

    // [254, 1]
    trigkey.begin = 254;
    trigkey.end = 1;
    tmp.emplace(std::make_pair(trigkey, 2));

    SnapTrigKey findKey = {2, 0};

    // auto iter = tmp.find(findKey);

    auto iter = std::find_if(tmp.begin(), tmp.end(), [&](const std::pair<SnapTrigKey, int> &trig) {
        const auto &key = trig.first;
        // 满足在区间内
        // [254, 255] -> 254, 255
        if (key.begin <= key.end) {
            if (key.begin <= findKey.begin && findKey.begin <= key.end) {
                return true;
            }
        } else {
            // [254, 1] -> 254, 255, 1
            // [255, 2] -> 255, 1, 2
            if ((findKey.begin <= 255 && findKey.begin >= key.begin) || (findKey.begin >= 1 && findKey.begin <= key.end)) {
                return true;
            }
        }
        return false;
    });

    if (iter != tmp.end()) {
        cout << "key: " << iter->first.begin << ", " << iter->first.end << endl;
        cout << "value: " << iter->second << endl;
    } else {
        cout << "not found" << endl;
    }
}

LTC_REGISTER_ACTION(ACTION_OTHERS, test_snapTrigkey);

#ifndef DIFFERENCE_H
#define DIFFERENCE_H

#include <vector>

// 差分数组工具类
class Difference {
public:
    Difference() {};
    virtual ~Difference() {};

    // 输入一个初始数组，区间操作将在这个数组上进行
    explicit Difference(std::vector<int> &nums) {
        if (nums.size() == 0) return;
        diff.resize(nums.size());
        diff[0] = nums[0];
        for (int i = 1; i < nums.size(); i++) {
            diff[i] = nums[i] - nums[i - 1];
        }
    }

    // 给闭区间 [i,j] 增加 val（可以是负数）
    void increment(int i, int j, int val) {
        diff[i] += val;
        if (j + 1 < diff.size()) {
            diff[j + 1] -= val;
        }
    }

    std::vector<int> result() {
        std::vector<int> res(diff.size());
        // 根据差分数组构造结果数组
        res[0] = diff[0];
        for (int i = 1; i < diff.size(); i++) {
            res[i] = res[i - 1] + diff[i];
        }
        return res;
    }
private:
    std::vector<int> diff;
};

#endif // DIFFERENCE_H
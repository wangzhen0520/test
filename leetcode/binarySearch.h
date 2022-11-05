#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include <vector>

int binarySearch(std::vector<int> &nums, int target);
int left_bound(std::vector<int> &nums, int target);
int right_bound(std::vector<int> &nums, int target);

#endif // BINARYSEARCH_H
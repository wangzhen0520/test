/*
82. 删除排序链表中的重复元素 II
给定一个已排序的链表的头 head ， 删除原始链表中所有重复数字的节点，只留下不同的数字 。返回 已排序的链表 。



示例 1：


输入：head = [1,2,3,3,4,4,5]
输出：[1,2,5]
示例 2：


输入：head = [1,1,1,2,3]
输出：[2,3]


提示：

链表中节点数目在范围 [0, 300] 内
-100 <= Node.val <= 100
题目数据保证链表已经按升序 排列
*/
#include "list_node.h"

#include "action.h"

#include <vector>

using namespace std;

class Solution {
public:
    ListNode *deleteDuplicates(ListNode *head) {
        return nullptr;
    }
};

void test_tree_82()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif
    Solution su;

    // 旋转数组查找
    vector<int> nums = {1, 2, 3, 3, 4, 4, 5};

    auto head = InitListNode(nums);
    PrintListNode(head);

    auto ret = su.deleteDuplicates(head);
    PrintListNode(ret);
}

LTC_REGISTER_ACTION(ACTION_LTC, test_tree_82);
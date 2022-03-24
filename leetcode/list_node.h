#ifndef LIST_NODE_H
#define LIST_NODE_H

#include <iostream>
#include <ostream>
#include <vector>

// Definition for singly-linked list.
struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

inline ListNode *InitListNode(std::vector<int> &nums)
{
    ListNode *head = new ListNode(-1);
    ListNode *tmp = head;
    for (int i = 0; i < nums.size(); i++) {
        ListNode *node = new ListNode(nums[i]);
        tmp->next = node;
        tmp = node;
    }

    return head->next;
}

inline void PrintListNode(ListNode *head)
{
    ListNode *p = head;
    while (p != nullptr) {
        std::cout << p->val << "->";
        p = p->next;
    }
    std::cout << std::endl;
}

#endif  // LIST_NODE_H
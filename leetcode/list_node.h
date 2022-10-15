#ifndef LIST_NODE_H
#define LIST_NODE_H

#include <iostream>
#include <vector>

// Definition for singly-linked list.
 struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

inline ListNode *MakeList(const std::vector<int> &data)
{
    ListNode *head = new ListNode();
    ListNode *p = head;
    for (const auto &iter : data) {
        ListNode *tmp = new ListNode(iter);
        p->next = tmp;
        p = tmp;
    }
    return head->next;
}

inline void PrintListNode(ListNode *head)
{
    ListNode *p = head;
    while (p) {
        std::cout << p->val;
        p = p->next;
        if (p != nullptr) {
            std::cout << "->";
        }
    }
    std::cout << std::endl;
}

#endif  // LIST_NODE_H
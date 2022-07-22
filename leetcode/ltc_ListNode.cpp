#include "action.h"

using namespace std;

struct ListNode
{
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
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
        }
        else {
            tmp->next = p2;
            p2 = p2->next;
        }
        tmp = tmp->next;
    }
    if (p1 == NULL) {
        tmp->next = p2;
    }
    else {
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
            }
            else {
                p->next = new ListNode(tmp / 10);
            }
        }
        p = p->next;
    }

    return l1;
}

void test_listNode()
{
#define test
#ifdef test
    return;
#else
    cout << __func__ << endl;
#endif

    ListNode ls11(1);
    ListNode ls12(2);
    ListNode ls13(4);

    ListNode ls21(1);
    ListNode ls22(3);
    ListNode ls23(4);
    // ListNode ls24(1);

    ListNode* l;
    ListNode* l1;
    ListNode* l2;

    l1 = &ls11;
    ls11.next = &ls12;
    ls12.next = &ls13;

    l2 = &ls21;
    ls21.next = &ls22;
    ls22.next = &ls23;
    // ls23.next = &ls24;

    // l = addTwoNumbers(l1, l2);
    l = mergeTwoLists(l1, l2);

    ListNode* p = l;
    while (p != NULL) {
        cout << p->val << endl;
        p = p->next;
    }
}
LTC_REGISTER_ACTION(ACTION_LTC, test_listNode);
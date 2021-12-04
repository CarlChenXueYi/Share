// @algorithm @lc id=21 lang=cpp
// @title merge-two-sorted-lists

#include "algm/algm.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
// @test([1,2,4],[1,3,4])=[1,1,2,3,4,4]
// @test([],[])=[]
// @test([],[0])=[0]

// Definition for singly-linked list.
// struct ListNode
// {
//     int       val;
//     ListNode* next;
//     ListNode() : val(0), next(nullptr) {}
//     ListNode(int x) : val(x), next(nullptr) {}
//     ListNode(int x, ListNode* next) : val(x), next(next) {}
// };

class Solution
{
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
    {
        ListNode* l3 = new ListNode{};
        if (l1 == nullptr)
            return l2;
        if (l2 == nullptr)
            return l1;
        if (l1->val < l2->val)
        {
            l3->val = l1->val;
            l1      = l1->next;
        }
        else
        {
            l3->val = l2->val;
            l2      = l2->next;
        }
        ListNode* l3temp = l3;
        while (l1 != nullptr && l2 != nullptr)
        {
            if (l1->val < l2->val)
            {
                ListNode l3cur = ListNode{l1->val, nullptr};

                l3temp->next = &l3cur;
                l3temp       = l3temp->next;
                l1           = l1->next;
            }
            else
            {
                ListNode l3cur = ListNode{l2->val, nullptr};

                l3temp->next = &l3cur;
                l3temp       = l3temp->next;
                l2           = l2->next;
            }
        }
        if (l1 != nullptr)
            l3->next = l1;
        if (l2 != nullptr)
            l2->next = l2;
        return l3;
    }
};
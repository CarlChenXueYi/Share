// @algorithm @lc id=23 lang=cpp
// @title merge-k-sorted-lists

#include "algm/algm.h"
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
// @test([[1,4,5],[1,3,4],[2,6]])=[1,1,2,3,4,4,5,6]
// @test([])=[]
// @test([[]])=[]

// Definition for singly-linked list.
struct ListNode
{
    int       val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution
{
public:
    struct cmp(ListNode* a, ListNode* b)
    {
        return a->val < b-> val;
    }
    ListNode* mergeKLists(vector<ListNode*>& lists)
    {
        // k个的结构是ListNode*，存在lists里面
        //
        priority_queue<ListNode*, vector<ListNode*>, greater<> pq;
    }
};
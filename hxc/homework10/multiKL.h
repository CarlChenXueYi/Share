#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

void setZero(int                     maxTask,
             int                     i,
             int                     tasksNum,
             vector<string>&         tasks,
             vector<vector<double>>& commCostMatrix,
             vector<int>&            nodes,
             int                     fn);
typedef struct
{
    int a;
    int b;
} selected;
class Task
{
private:
    int                    oriTaskNum;
    int                    num;
    vector<string>         name;
    vector<int>            nodes;
    vector<vector<double>> c;
    vector<int>            A, B;

public:
    Task(int ornu, int n, vector<string> d, vector<vector<double>> a) : oriTaskNum(ornu), num(n), name(d), c(a)
    {
        for (int i = 0; i < num; i++)
        {
            nodes.push_back(i);
        }
    }
    Task(int ornu, int n, vector<string> d, vector<int> no, vector<vector<double>> a)
        : oriTaskNum(ornu), num(n), name(d), nodes(no), c(a)
    {
    }
    void        Binary();
    void        printAB(vector<int> A, vector<int> B, int times);
    void        calCost();
    vector<int> getA()
    {
        return A;
    }
    vector<int> getB()
    {
        return B;
    }
};

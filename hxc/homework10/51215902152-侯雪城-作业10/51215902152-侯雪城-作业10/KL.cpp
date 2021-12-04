#include "KL.h"
#include "Task.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

map<int, string> tasksMP;
int              main()
{
    for (int i = 0; i < 100; i++)
    {
        vector<int>            moduleA, moduleB;
        vector<double>         D;
        vector<vector<double>> commCostMatrix;
        vector<string>         tasks;

        GetInfo("7_2", commCostMatrix, tasks);

        int taskNum = tasks.size();
        cout << "第" << i + 1 << "初始生成划分如下：" << endl;

        for (int i = 0; i < taskNum; i++)
        {
            tasksMP[i] = tasks[i];
        }
        for (int i = 0; i < taskNum; i++)
        {
            D.push_back(0);
        }
        randSelect(moduleA, moduleB, taskNum);
        cout << "A：";
        for (auto ait = moduleA.begin(); ait != moduleA.end(); ait++)
        {
            cout << tasksMP[*ait] << " ";
        }

        cout << endl;
        cout << "B：";
        for (auto bit = moduleB.begin(); bit != moduleB.end(); bit++)
        {
            cout << tasksMP[*bit] << " ";
        }
        cout << endl;

        KL(taskNum, D, moduleA, moduleB, commCostMatrix);

        cout << "通信代价为：" << sumCommCost(moduleA, moduleB, commCostMatrix) << endl << endl << endl;
    }

    return 0;
}

// 1. 计算所有任务的D值 （E-I； E是全部外部成本，I是全部内部成本）
void caculateD(vector<bool>&           curTask,
               int                     index,
               vector<double>&         D,
               vector<int>&            moduleA,
               vector<int>&            moduleB,
               vector<vector<double>>& commCostMatrix)
{
    if (!curTask[index])
        return;
    auto it = find(moduleA.begin(), moduleA.end(), index);

    if (it != moduleA.end())
    {
        // index在A里面，A内的是内部，A外的是外部
        double E = 0, I = 0;
        for (auto ait = moduleA.begin(); ait != moduleA.end(); ait++)
        {
            if (curTask[*ait])
                I += commCostMatrix[index][*ait];
        }
        for (auto bit = moduleB.begin(); bit != moduleB.end(); bit++)
        {
            if (curTask[*bit])
                E += commCostMatrix[index][*bit];
        }
        D[index] = E - I;
    }
    else
    {

        double E = 0, I = 0;
        for (auto ait = moduleA.begin(); ait != moduleA.end(); ait++)
        {
            if (curTask[*ait])
                E += commCostMatrix[index][*ait];
        }
        for (auto bit = moduleB.begin(); bit != moduleB.end(); bit++)
        {
            if (curTask[*bit])
                I += commCostMatrix[index][*bit];
        }
        D[index] = E - I;
    }
    return;
}

// 2. A中精选a，B中精选b，返回结果是ai下标，bi下标，以及gain
pair<pair<int, int>, double> selectedElement(vector<bool>&           curTask,
                                             vector<double>&         D,
                                             vector<int>&            moduleA,
                                             vector<int>&            moduleB,
                                             vector<vector<double>>& commCostMatrix)
{
    int    ai, bi;
    double gain = -1000000.0;

    for (auto ait = moduleA.begin(); ait != moduleA.end(); ait++)
    {
        for (auto bit = moduleB.begin(); bit != moduleB.end(); bit++)
        {
            if (curTask[*ait] && curTask[*bit])
            {
                if (D[*ait] + D[*bit] - 2 * commCostMatrix[*ait][*bit] > gain)
                {
                    ai   = *ait;
                    bi   = *bit;
                    gain = D[*ait] + D[*bit] - 2 * commCostMatrix[*ait][*bit];
                }
            }
        }
    }
    return pair<pair<int, int>, double>{{ai, bi}, gain};
}

void KL(int                     taskNum,
        vector<double>&         D,
        vector<int>&            moduleA,
        vector<int>&            moduleB,
        vector<vector<double>>& commCostMatrix)
{

    vector<pair<pair<int, int>, double>> rst;
    vector<bool>                         curTask(taskNum, true);
    for (int i = 0; i < taskNum; i++)
    {
        caculateD(curTask, i, D, moduleA, moduleB, commCostMatrix);
    }

    int p = 1;
    while (p != taskNum / 2 + 1)
    {
        pair<pair<int, int>, double> selected = selectedElement(curTask, D, moduleA, moduleB, commCostMatrix);
        rst.push_back(selected);
        curTask[selected.first.first]  = false;
        curTask[selected.first.second] = false;
        for (int i = 0; i < taskNum; i++)
        {
            caculateD(curTask, i, D, moduleA, moduleB, commCostMatrix);
        }
        p++;
    }
    int    k   = 0;
    double G   = 0;
    double max = 1.0 * (INT32_MIN);
    for (int i = 0; i < taskNum / 2; i++)
    {
        // cout << "gain" << rst[i].second << " ";
        G += rst[i].second;
        if (G > max)
        {
            max = G;
            k   = i;
        }
    }

    G = 0;
    for (int i = 0; i <= k; i++)
        G += rst[i].second;

    if (G > 0)
    {
        for (int i = 0; i <= k; i++)
        {
            //这里可能导致moduleA与moduleB中的序号是乱序的
            int temp = rst[i].first.first;
            moduleA.erase(remove(moduleA.begin(), moduleA.end(), temp));
            // moduleA.erase(moduleA.begin() + rst[i].first.first);
            moduleB.push_back(temp);

            temp = rst[i].first.second;
            moduleB.erase(remove(moduleB.begin(), moduleB.end(), temp));
            moduleA.push_back(temp);

            //重新调用主函数
        }
        KL(taskNum, D, moduleA, moduleB, commCostMatrix);
    }
    else
    {
        cout << "KL调整后划分如下：" << endl;
        cout << "A：";
        for (auto ait = moduleA.begin(); ait != moduleA.end(); ait++)
        {
            cout << tasksMP[*ait] << " ";
        }
        cout << endl;
        cout << "B：";
        for (auto bit = moduleB.begin(); bit != moduleB.end(); bit++)
        {
            cout << tasksMP[*bit] << " ";
        }
        cout << endl;
        return;
    }
}

void randSelect(vector<int>& moduleA, vector<int>& moduleB, int taskNum)
{
    int arr[taskNum];
    for (int i = 0; i < taskNum; i++)
        arr[i] = 1; //生成100个不同的数，这里直接用a[i]=i;
    sleep(1);
    srand((unsigned)time(NULL)); //设置随机种子
    int index = 0;
    while (index < taskNum / 2)
    {
        int t = rand() % taskNum;
        if (arr[t] != 0)
        {
            moduleA.push_back(t);
            arr[t] = 0;
            index++;
        }
    }

    for (int i = 0; i < taskNum; i++)
    {
        if (arr[i] != 0)
        {
            moduleB.push_back(i);
        }
    }

    return;
}

double sumCommCost(vector<int>& moduleA, vector<int>& moduleB, vector<vector<double>>& commCostMatrix)
{
    double sum = 0;
    for (auto ait = moduleA.begin(); ait != moduleA.end(); ait++)
    {
        for (auto bit = moduleB.begin(); bit != moduleB.end(); bit++)
        {
            sum += commCostMatrix[*ait][*bit];
        }
    }
    return sum;
}
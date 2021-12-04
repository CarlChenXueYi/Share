#include "multiKL.h"
#include "Task.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> ret;

void KLToExpon(int oriTaskNum, int n, vector<string> d, vector<vector<double>> a, vector<int> nodes, int k)
{
    if (k == 0)
    {
        ret.push_back(nodes);
        return;
    }
    Task kl(oriTaskNum, n, d, nodes, a);
    kl.Binary();
    k--;
    KLToExpon(oriTaskNum, n >> 1, d, a, kl.getA(), k);
    KLToExpon(oriTaskNum, n >> 1, d, a, kl.getB(), k);
}

int main()
{
    vector<vector<double>> commCostMatrix;
    vector<string>         tasks;
    GetInfo("7_11", commCostMatrix, tasks);
    int tasksNum = tasks.size();

    int k       = 3;
    int maxTask = 4;

    int i = 1;
    while (k > (1 << i))
    {
        i++;
    }
    if (tasksNum > maxTask * (1 << i))
    {
        cout << "无法划分！" << endl;
    }
    else
    {
        int         fn = maxTask * (1 << i);
        vector<int> nodes;
        setZero(maxTask, i, tasksNum, tasks, commCostMatrix, nodes, fn);

        KLToExpon(fn, fn, tasks, commCostMatrix, nodes, i);
        for (int j = 0; j < ret.size(); j++)
        {
            for (auto x = ret[j].begin(); x < ret[j].end(); x++)
            {
                if (tasks[*x] == " ")
                {
                    ret[j].erase(x);
                    x--;
                }
            }
            if (ret[j].size() == 0)
            {
                ret.erase(ret.begin() + j);
            }
        }

        for (int j = 0; j < ret.size(); j++)
        {
            cout << "{";
            for (auto x : ret[j])
            {
                cout << tasks[x];
                if (x != *(ret[j].end() - 1))
                    cout << ",";
            }
            cout << "}";
        }
        cout << endl;

        for (int j = 0; j < ret.size(); j++)
        {
            for (int k = j + 1; k < ret.size(); k++)
            {
                cout << "第" << j + 1 << "个模块和"
                     << "第" << k + 1 << "个模块的通讯代价为";
                int sum = 0;
                for (auto x : ret[j])
                {
                    for (auto y : ret[k])
                    {
                        sum += commCostMatrix[x][y];
                    }
                }
                cout << sum << endl;
            }
        }
    }
    return 0;
}

void setZero(int                     maxTask,
             int                     i,
             int                     tasksNum,
             vector<string>&         tasks,
             vector<vector<double>>& commCostMatrix,
             vector<int>&            nodes,
             int                     fn)
{
    for (int j = tasksNum; j < fn; j++)
    {
        tasks.push_back(" ");
    }
    for (int j = 0; j < tasksNum; j++)
    {
        for (int k = tasksNum; k < fn; k++)
        {
            commCostMatrix[j].push_back(0);
        }
    }
    for (int j = tasksNum; j < fn; j++)
    {
        commCostMatrix.push_back(vector<double>(fn, 0));
    }
    for (int j = 0; j < fn; j++)
    {
        nodes.push_back(j);
    }
}
void Task::Binary()
{
    A = vector<int>(nodes.begin(), nodes.begin() + (num >> 1));
    B = vector<int>(nodes.begin() + (num >> 1), nodes.end());
    int G;
    int times = 1;
    do
    {
        vector<float>    gain(num / 2, INT32_MIN);
        vector<selected> selet(num / 2);
        vector<int>      tempA(A), tempB(B);
        for (int i = 0; i < A.size(); i++)
        {

            vector<float> D(oriTaskNum, 0);
            for (auto x : tempA)
            {
                float tempE = 0, tempI = 0;
                for (auto y : tempB)
                {
                    tempE += c[x][y];
                }
                for (auto y : tempA)
                {
                    tempI += c[x][y];
                }
                D[x] = tempE - tempI;
            }
            for (auto x : tempB)
            {
                float tempE = 0, tempI = 0;
                for (auto y : tempA)
                {
                    tempE += c[x][y];
                }
                for (auto y : tempB)
                {
                    tempI += c[x][y];
                }
                D[x] = tempE - tempI;
            }
            for (auto x : tempA)
            {
                for (auto y : tempB)
                {
                    if (D[x] + D[y] - 2 * c[x][y] > gain[i])
                    {
                        gain[i]    = D[x] + D[y] - 2 * c[x][y];
                        selet[i].a = x;
                        selet[i].b = y;
                    }
                }
            }

            tempA.erase(find(tempA.begin(), tempA.end(), selet[i].a));
            tempB.erase(find(tempB.begin(), tempB.end(), selet[i].b));
        }

        G = 0;
        for (int i = 0; i < gain.size(); i++)
        {
            if (gain[i] > 0)
            {
                A.erase(find(A.begin(), A.end(), selet[i].a));
                B.erase(find(B.begin(), B.end(), selet[i].b));
                A.push_back(selet[i].b);
                B.push_back(selet[i].a);
                G = G + gain[i];
            }
        }

    } while (G > 0);
    calCost();
}

void Task::calCost()
{
    float sum = 0;
    for (auto x : A)
    {
        for (auto y : B)
        {
            sum += c[x][y];
        }
    }
}
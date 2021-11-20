#include "Task.h"
#include "cmdline.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

vector<vector<string>> initModObj(vector<string>& tasks);
void                   makeMap(map<string, int>& tasksToIndex, vector<string>& tasks);
void                   agglomerative(int                           mode,
                                     const vector<string>&               tasks,
                                     const vector<vector<string>>& k1Tasks,
                                     vector<vector<int>>&          commCostMatrix,
                                     int                           moduleNum,
                                     int                           maxTaskNumInClu,
                                     map<string, int>&             tasksToIndex);
bool                   singleAgglomerative(vector<vector<string>>::iterator formerIt,
                                           vector<vector<string>>::iterator laterIt,
                                           map<string, int>&                tasksToIndex,
                                           vector<vector<int>>&             commCostMatrix,
                                           int                              thold);
bool                   completeAgglomerative(vector<vector<string>>::iterator formerIt,
                                             vector<vector<string>>::iterator laterIt,
                                             map<string, int>&                tasksToIndex,
                                             vector<vector<int>>&             commCostMatrix,
                                             int                              thold);
bool                   averageAgglomerative(vector<vector<string>>::iterator formerIt,
                                            vector<vector<string>>::iterator laterIt,
                                            map<string, int>&                tasksToIndex,
                                            vector<vector<int>>&             commCostMatrix,
                                            int                              thold);
int                    getMaxCommCost(vector<vector<int>>& commCostMatrix);
void                   tableInit();
void                   tableFormat();
void                   runTimeOutPut(int thold, int moduleNum, std::vector<std::vector<std::string>> modObj);
void                   singleAgglomerativeK1(const vector<vector<int>>&    commCostMatrix,
                                             const vector<vector<string>>& modObj,
                                             map<string, int>&             tasksToIndex,
                                             int                           moduleNum,
                                             int                           maxTaskNumInClu);

struct dendrogram
{
    int                    thold;
    int                    tempModNum;
    vector<vector<string>> modObj;
};
int main(int argc, char* argv[])
{
    vector<vector<int>> commCostMatrix;
    vector<string>      tasks;

    GetInfo("7_4", commCostMatrix, tasks);

    map<string, int> tasksToIndex;
    makeMap(tasksToIndex, tasks);

    cmdline::parser aa;
    aa.add<int>("moduleNum", 'm', "the number of module you want to divide", true);
    aa.add<int>("maxTaskNumInClu", 't', "the max number of task in a module", true);
    aa.parse_check(argc, argv);
    int moduleNum       = aa.get<int>("moduleNum");
    int maxTaskNumInClu = aa.get<int>("maxTaskNumInClu");

    tableInit();
    // mode含义 ： 1. 单链接 2. 全链接 3. 均链接
    // vector<vector<string>> nothing;
    agglomerative(2, tasks, vector<vector<string>>{}, commCostMatrix, moduleNum, maxTaskNumInClu, tasksToIndex);

    tableFormat();
    return 0;
}

void makeMap(map<string, int>& tasksToIndex, vector<string>& tasks)
{
    int i = 0;
    for (auto it = tasks.begin(); it != tasks.end(); it++)
    {
        tasksToIndex[*it] = i;
        i++;
    }
}

void agglomerative(int                           mode,
                   vector<string>&               tasks,
                   const vector<vector<string>>& k1Tasks,
                   vector<vector<int>>&          commCostMatrix,
                   int                           moduleNum,
                   int                           maxTaskNumInClu,
                   map<string, int>&             tasksToIndex)
{
    cout << "enter agglomerative function" << endl;
    int thold = getMaxCommCost(commCostMatrix);

    // return ;
    int tempModNum = tasks.size();
    // modObj就是任务属于哪些模块的具体划分；
    vector<vector<string>> modObj  = initModObj(tasks, k1Tasks);
    dendrogram             de      = dendrogram{thold, tempModNum, modObj};
    dendrogram             firstDe = dendrogram{thold, tempModNum, modObj};
    vector<dendrogram>     deList  = {firstDe};
    while (thold != 0 && tempModNum != moduleNum)
    {
        for (auto formerIt = de.modObj.begin(); formerIt != de.modObj.end(); formerIt++)
        {

            auto laterIt = formerIt + 1;
            while (laterIt != de.modObj.end())
            {
                bool isMerge;
                switch (mode)
                {
                    case 1:
                        isMerge = singleAgglomerative(formerIt, laterIt, tasksToIndex, commCostMatrix, thold);
                        break;
                    case 2:
                        isMerge = completeAgglomerative(formerIt, laterIt, tasksToIndex, commCostMatrix, thold);
                        break;
                    case 3:
                        isMerge = averageAgglomerative(formerIt, laterIt, tasksToIndex, commCostMatrix, thold);
                        break;
                    default:
                        break;
                }
                //  = singleAgglomerative(formerIt, laterIt, tasksToIndex, commCostMatrix, thold);
                // bool isMerge = completeAgglomerative(formerIt, laterIt, tasksToIndex, commCostMatrix, thold);
                // bool isMerge = averageAgglomerative(formerIt, laterIt, tasksToIndex, commCostMatrix, thold);
                if (isMerge && formerIt->size() + laterIt->size() <= maxTaskNumInClu)
                {
                    for (auto it = laterIt->begin(); it != laterIt->end(); it++)
                    {
                        formerIt->push_back(*it);
                    }
                    de.modObj.erase(laterIt);
                    // laterIt++;
                }
                else
                    laterIt++;
            }
        }
        tempModNum = de.modObj.size();
        deList.push_back({thold - 1, tempModNum, de.modObj});
        runTimeOutPut(thold, tempModNum, de.modObj);
        thold--;
        if (thold < -1)
            return;
        // cout << "thold " << thold << endl;
    }
    if (thold == 0 && tempModNum > moduleNum)
    {
        singleAgglomerativeK1(commCostMatrix, de.modObj, tasksToIndex, moduleNum, maxTaskNumInClu);
    }
}

bool singleAgglomerative(vector<vector<string>>::iterator formerIt,
                         vector<vector<string>>::iterator laterIt,
                         map<string, int>&                tasksToIndex,
                         vector<vector<int>>&             commCostMatrix,
                         int                              thold)
{
    for (int i = 0; i < formerIt->size(); i++)
    {
        for (int j = 0; j < laterIt->size(); j++)
        {
            // string str = (*formerIt)[i];
            if (commCostMatrix[tasksToIndex[(*formerIt)[i]]][tasksToIndex[(*laterIt)[j]]] >= thold)
                return true;
        }
    }
    return false;
}
bool completeAgglomerative(vector<vector<string>>::iterator formerIt,
                           vector<vector<string>>::iterator laterIt,
                           map<string, int>&                tasksToIndex,
                           vector<vector<int>>&             commCostMatrix,
                           int                              thold)
{
    for (int i = 0; i < formerIt->size(); i++)
    {
        for (int j = 0; j < laterIt->size(); j++)
        {
            // string str = (*formerIt)[i];
            if (commCostMatrix[tasksToIndex[(*formerIt)[i]]][tasksToIndex[(*laterIt)[j]]] < thold)
                return false;
        }
    }
    return true;
}
bool averageAgglomerative(vector<vector<string>>::iterator formerIt,
                          vector<vector<string>>::iterator laterIt,
                          map<string, int>&                tasksToIndex,
                          vector<vector<int>>&             commCostMatrix,
                          int                              thold)
{
    int sum = 0;
    for (int i = 0; i < formerIt->size(); i++)
    {
        for (int j = 0; j < laterIt->size(); j++)
        {
            sum += commCostMatrix[tasksToIndex[(*formerIt)[i]]][tasksToIndex[(*laterIt)[j]]];
        }
    }
    if (sum / (formerIt->size() * laterIt->size()) >= thold)
        return true;
    return false;
}

vector<vector<string>> initModObj(vector<string>& tasks, vector<vector<string>>& k1Tasks)
{
    if (tasks.size() != 0)
    {
        vector<vector<string>> modObj;
        for (auto it = tasks.begin(); it != tasks.end(); it++)
        {
            vector<string> tempName;
            tempName.push_back(*it);
            modObj.push_back(tempName);
        }
        return modObj;
    }
    else if (k1Tasks.size() != 0)
    {
        return k1Tasks;
    }
}

//获取通信矩阵中最大值
int getMaxCommCost(vector<vector<int>>& commCostMatrix)
{
    int max = -1;
    for (auto it = commCostMatrix.begin(); it != commCostMatrix.end(); it++)
    {
        for (auto bit = it->begin(); bit != it->end(); bit++)
        {
            if (*bit > max)
                max = *bit;
        }
    }
    return max;
}

void singleAgglomerativeK1(const vector<vector<int>>&    commCostMatrix,
                           const vector<vector<string>>& modObj,
                           map<string, int>&             tasksToIndex,
                           int                           moduleNum,
                           int                           maxTaskNumInClu)
{
    //根据原先的二维矩阵，以及现在的划分情况，计算出一个新的二维矩阵
    int k1CommNum = modObj.size();
    cout << "k1NUm " << k1CommNum << endl;
    vector<vector<int>> k1CommCostMatrix;
    for (auto ait = modObj.begin(); ait != modObj.end(); ait++)
    {
        vector<int> tempCommCost;
        for (auto bit = modObj.begin(); bit != modObj.end(); bit++)
        {
            int sum = 0;
            for (auto mit = ait->begin(); mit != ait->end(); mit++)
            {
                cout << "mit " << *mit << endl;
                for (auto nit = bit->begin(); nit != bit->end(); nit++)
                {
                    cout << "nit " << *nit << endl;
                    cout << "taskindex mit " << tasksToIndex[*mit] << "taskindex nit " << tasksToIndex[*nit] << endl;
                    sum += commCostMatrix[tasksToIndex[*mit]][tasksToIndex[*nit]];
                }
            }
            if (ait == bit)
                sum = 0;
            tempCommCost.push_back(sum);
        }
        k1CommCostMatrix.push_back(tempCommCost);
    }
    /*
    ! 这个地方还需要修改一些
    */
    vector<string> k1Tasks;
    for (auto it = modObj.begin(); it != modObj.end(); it++)
    {
        // string temp;
        // for (auto bit = it->begin(); bit != it->end(); bit++)
        // {
        //     temp.append(*bit);
        //     if (bit != it->end() - 1)
        //         temp.append(",");
        // }
        k1Tasks.push_back(it->front());
    }
    map<string, int> k1TasksToIndex;
    makeMap(k1TasksToIndex, k1Tasks);
    agglomerative(1, vector<string>{},k1Tasks, k1CommCostMatrix, moduleNum, maxTaskNumInClu, k1TasksToIndex);
    //输出获得的新矩阵
    for (auto ait = k1CommCostMatrix.begin(); ait != k1CommCostMatrix.end(); ait++)
    {
        for (auto bit = ait->begin(); bit != ait->end(); bit++)
        {
            cout << *bit << " ";
        }
        cout << endl;
    }
}
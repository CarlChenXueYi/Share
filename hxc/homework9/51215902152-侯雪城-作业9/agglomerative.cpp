#include "agglomerative.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    vector<vector<int>> commCostMatrix;
    vector<string>      tasks;

    GetInfo("7_7", commCostMatrix, tasks);

    map<string, int> tasksToIndex;
    makeMap(tasksToIndex, tasks, vector<vector<string>>{});

    cmdline::parser aa;
    aa.add<int>("moduleNum", 'n', "the number of module you want to divide", true);
    aa.add<int>("maxTaskNumInClu", 't', "the max number of task in a module", true);
    aa.add<int>("mode", 'm', "decide use which method to merge", true);
    aa.parse_check(argc, argv);
    int moduleNum       = aa.get<int>("moduleNum");
    int maxTaskNumInClu = aa.get<int>("maxTaskNumInClu");
    int mode            = aa.get<int>("mode");
    tableInit();
    // mode含义 ： 1. 单链接 2. 全链接 3. 均链接
    // vector<vector<string>> nothing;
    agglomerative(mode, tasks, commCostMatrix, moduleNum, maxTaskNumInClu, tasksToIndex);

    tableFormat();
    return 0;
}

void makeMap(map<string, int>& tasksToIndex, const vector<string>& tasks, const vector<vector<string>>& k1Tasks)
{
    if (tasks.size() != 0)
    {
        int i = 0;
        for (auto it = tasks.begin(); it != tasks.end(); it++)
        {
            tasksToIndex[*it] = i;
            i++;
        }
    }
    else if (k1Tasks.size() != 0)
    {
        int i = 0;
        for (auto it = k1Tasks.begin(); it != k1Tasks.end(); it++)
        {
            tasksToIndex[it->front()] = i;
            i++;
        }
    }
}

void agglomerative(int                  mode,
                   vector<string>&      tasks,
                   vector<vector<int>>& commCostMatrix,
                   int                  moduleNum,
                   int                  maxTaskNumInClu,
                   map<string, int>&    tasksToIndex)
{
    int thold = getMaxCommCost(commCostMatrix);

    // return ;
    int tempModNum = tasks.size();
    // modObj就是任务属于哪些模块的具体划分；
    vector<vector<string>> modObj  = initModObj(tasks, vector<vector<string>>{});
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
    finOutPut(de.modObj, commCostMatrix, tasksToIndex);
}

// void k1Agglomerative(const vector<vector<string>>& k1Tasks,
//                      vector<vector<int>>&          commCostMatrix,
//                      int                           moduleNum,
//                      int                           maxTaskNumInClu,
//                      map<string, int>&             tasksToIndex)
// {
//     int thold = getMaxCommCost(commCostMatrix);

//     // return ;
//     int tempModNum = k1Tasks.size();
//     // modObj就是任务属于哪些模块的具体划分；
//     vector<vector<string>> modObj  = initModObj(vector<string>{}, k1Tasks);
//     dendrogram             de      = dendrogram{thold, tempModNum, modObj};
//     dendrogram             firstDe = dendrogram{thold, tempModNum, modObj};
//     vector<dendrogram>     deList  = {firstDe};
//     while (thold != 0 && tempModNum != moduleNum)
//     {
//         for (auto formerIt = de.modObj.begin(); formerIt != de.modObj.end(); formerIt++)
//         {
//             auto laterIt = formerIt + 1;
//             while (laterIt != de.modObj.end())
//             {
//                 bool isMerge = k1SingleAgglomerative(formerIt, laterIt, tasksToIndex, commCostMatrix, thold);
//                 if (isMerge && formerIt->size() + laterIt->size() <= maxTaskNumInClu)
//                 {
//                     for (auto it = laterIt->begin(); it != laterIt->end(); it++)
//                     {
//                         formerIt->push_back(*it);
//                     }
//                     de.modObj.erase(laterIt);
//                     // laterIt++;
//                 }
//                 else
//                     laterIt++;
//             }
//         }
//         tempModNum = de.modObj.size();
//         deList.push_back({thold - 1, tempModNum, de.modObj});
//         runTimeOutPut(thold, tempModNum, de.modObj);
//         thold--;
//     }
//     if (thold == 0 && tempModNum > moduleNum)
//     {
//         cout << "wtf ? I will figure it out!" << endl;
//     }
// }

void k1Agglomerative(vector<vector<string>>& k1Tasks,
                     vector<vector<int>>&          commCostMatrix,
                     int                           moduleNum,
                     int                           maxTaskNumInClu,
                     map<string, int>&             tasksToIndex)
{
    int thold = 1;

    // return ;
    int tempModNum = k1Tasks.size();
    // modObj就是任务属于哪些模块的具体划分；
    //vector<vector<string>> modObj  = initModObj(vector<string>{}, k1Tasks);
    //dendrogram             de      = dendrogram{thold, tempModNum, modObj};
    //dendrogram             firstDe = dendrogram{thold, tempModNum, modObj};
    //vector<dendrogram>     deList  = {firstDe};
    while (thold != 0 && tempModNum != moduleNum)
    {
        for (auto formerIt = k1Tasks.begin(); formerIt != k1Tasks.end(); formerIt++)
        {
            auto laterIt = formerIt + 1;
            while (laterIt != k1Tasks.end())
            {
                bool isMerge = k1SingleAgglomerative(formerIt, laterIt, tasksToIndex, commCostMatrix, thold);
                if (isMerge && formerIt->size() + laterIt->size() <= maxTaskNumInClu)
                {
                    for (auto it = laterIt->begin(); it != laterIt->end(); it++)
                    {
                        formerIt->push_back(*it);
                    }
                    k1Tasks.erase(laterIt);
                    // laterIt++;
                }
                else
                    laterIt++;
            }
        }
        tempModNum = k1Tasks.size();
        //deList.push_back({thold - 1, tempModNum, de.modObj});
        runTimeOutPut(thold, tempModNum, k1Tasks);
        thold--;
    }
    if (thold == 0 && tempModNum > moduleNum)
    {
        cout << "wtf ? I will figure it out!" << endl;
    }
}

bool k1SingleAgglomerative(vector<vector<string>>::iterator formerIt,
                           vector<vector<string>>::iterator laterIt,
                           map<string, int>&                tasksToIndex,
                           vector<vector<int>>&             commCostMatrix,
                           int                              thold)
{
    for (int i = 0; i < formerIt->size(); i++)
    {
        for (int j = 0; j < laterIt->size(); j++)
        {
            if (tasksToIndex.count((*formerIt)[i]) > 0 && tasksToIndex.count((*laterIt)[j]) > 0)
            {
                if (commCostMatrix[tasksToIndex[(*formerIt)[i]]][tasksToIndex[(*laterIt)[j]]] >= thold)
                    return true;
            }
        }
    }
    return false;
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

vector<vector<string>> initModObj(const vector<string>& tasks, const vector<vector<string>>& k1Tasks)
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
    return vector<vector<string>>{};
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
                           vector<vector<string>>& modObj,
                           map<string, int>&             tasksToIndex,
                           int                           moduleNum,
                           int                           maxTaskNumInClu)
{
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
                for (auto nit = bit->begin(); nit != bit->end(); nit++)
                {
                    sum += commCostMatrix[tasksToIndex[*mit]][tasksToIndex[*nit]];
                }
            }
            if (ait == bit)
                sum = 0;
            tempCommCost.push_back(sum);
        }
        k1CommCostMatrix.push_back(tempCommCost);
    }

    map<string, int> k1TasksToIndex;

    makeMap(k1TasksToIndex, vector<string>{}, modObj);

    k1Agglomerative(modObj, k1CommCostMatrix, moduleNum, maxTaskNumInClu, k1TasksToIndex);
    //输出获得的新矩阵
    // for (auto ait = k1CommCostMatrix.begin(); ait != k1CommCostMatrix.end(); ait++)
    // {
    //     for (auto bit = ait->begin(); bit != ait->end(); bit++)
    //     {
    //         cout << *bit << " ";
    //     }
    //     cout << endl;
    // }
}
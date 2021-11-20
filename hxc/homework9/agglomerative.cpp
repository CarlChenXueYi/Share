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
void                   agglomerative(vector<string>&      tasks,
                                     vector<vector<int>>& commCostMatrix,
                                     int                  moduleNum,
                                     int                  maxTaskNumInClu,
                                     map<string, int>&    tasksToIndex);
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
bool                   averagelomerative(vector<vector<string>>::iterator formerIt,
                                         vector<vector<string>>::iterator laterIt,
                                         map<string, int>&                tasksToIndex,
                                         vector<vector<int>>&             commCostMatrix,
                                         int                              thold);
int                    getMaxCommCost(vector<vector<int>>& commCostMatrix);
void                   tableInit();
void                   tableFormat();
void                   runTimeOutPut(int thold, int moduleNum, std::vector<std::vector<std::string>> modObj);

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

    agglomerative(tasks, commCostMatrix, moduleNum, maxTaskNumInClu, tasksToIndex);

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

void agglomerative(vector<string>&      tasks,
                   vector<vector<int>>& commCostMatrix,
                   int                  moduleNum,
                   int                  maxTaskNumInClu,
                   map<string, int>&    tasksToIndex)
{
    int thold = getMaxCommCost(commCostMatrix);

    // return ;
    int tempModNum = tasks.size();
    // modObj就是任务属于哪些模块的具体划分；
    vector<vector<string>> modObj  = initModObj(tasks);
    dendrogram             de      = dendrogram{thold, tempModNum, modObj};
    dendrogram             firstDe = dendrogram{thold, tempModNum, modObj};
    vector<dendrogram>     deList  = {firstDe};
    while (thold != 0 || tempModNum != moduleNum)
    {
        for (auto formerIt = de.modObj.begin(); formerIt != de.modObj.end(); formerIt++)
        {
            auto laterIt = formerIt + 1;
            while (laterIt != de.modObj.end())
            {
                bool isMerge = singleAgglomerative(formerIt, laterIt, tasksToIndex, commCostMatrix, thold);
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
    }
    if (thold==0&&tempModNum>moduleNum)
    {
        singleAgglomerativeK1(现在的划分的obj，原来的通信矩阵，阈值需要重新算了，要求划分的模块数以及每个模块中最大的任务数)
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

vector<vector<string>> initModObj(vector<string>& tasks)
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
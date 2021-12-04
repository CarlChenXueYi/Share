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

vector<vector<string>> initModObj(const vector<string>& tasks, const vector<vector<string>>& k1Tasks);
void makeMap(map<string, int>& tasksToIndex, const vector<string>& tasks, const vector<vector<string>>& k1Tasks);
void agglomerative(int                  mode,
                   vector<string>&      tasks,
                   vector<vector<int>>& commCostMatrix,
                   int                  moduleNum,
                   int                  maxTaskNumInClu,
                   map<string, int>&    tasksToIndex);

void k1Agglomerative(vector<vector<string>>& k1Tasks,
                     vector<vector<int>>&          commCostMatrix,
                     int                           moduleNum,
                     int                           maxTaskNumInClu,
                     map<string, int>&             tasksToIndex);
bool singleAgglomerative(vector<vector<string>>::iterator formerIt,
                         vector<vector<string>>::iterator laterIt,
                         map<string, int>&                tasksToIndex,
                         vector<vector<int>>&             commCostMatrix,
                         int                              thold);
bool completeAgglomerative(vector<vector<string>>::iterator formerIt,
                           vector<vector<string>>::iterator laterIt,
                           map<string, int>&                tasksToIndex,
                           vector<vector<int>>&             commCostMatrix,
                           int                              thold);
bool averageAgglomerative(vector<vector<string>>::iterator formerIt,
                          vector<vector<string>>::iterator laterIt,
                          map<string, int>&                tasksToIndex,
                          vector<vector<int>>&             commCostMatrix,
                          int                              thold);
int  getMaxCommCost(vector<vector<int>>& commCostMatrix);
void tableInit();
void tableFormat();
void runTimeOutPut(int thold, int moduleNum, std::vector<std::vector<std::string>> modObj);
void singleAgglomerativeK1(const vector<vector<int>>&    commCostMatrix,
                           vector<vector<string>>& modObj,
                           map<string, int>&             tasksToIndex,
                           int                           moduleNum,
                           int                           maxTaskNumInClu);
bool k1SingleAgglomerative(vector<vector<string>>::iterator formerIt,
                           vector<vector<string>>::iterator laterIt,
                           map<string, int>&                tasksToIndex,
                           vector<vector<int>>&             commCostMatrix,
                           int                              thold);
void finOutPut(std::vector<std::vector<std::string>> modObj,
          std::vector<std::vector<int>>&        commCostMatrix,
          std::map<std::string, int>&           tasksToIndex);
struct dendrogram
{
    int                    thold;
    int                    tempModNum;
    vector<vector<string>> modObj;
};
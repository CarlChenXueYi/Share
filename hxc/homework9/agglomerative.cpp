#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

struct dendrogram {
    int thold;
    int tempModNum;
    vector<vector<string>> modObj;
};
int main()
{
    
    return 0;
}


void singleAgglomerative(vector<string>& tasks,vector<vector<int>>& commCostMatrix,int moduleNum,int maxTaskNumInClu)
{
    int thold = getMaxCommCost(commCostMatrix);
    int tempModNum = tasks.size();
    //modObj就是任务属于哪些模块的具体划分；
    vector<vector<string>> modObj = initModObj(tasks);
    dendrogram de = dendrogram{thold,tempModNum,modObj};
    for (auto formerIt=de.modObj.begin();formerIt!=de.modObj.end();formerIt++)
    {
        auto laterIt = formerIt+1;
        for (auto laterIt = formerIt+1;laterIt!= de.modObj.end();formerIt++)
        {
            
        }
        while ()

    }

}

vector<vector<string>> initModObj(vector<string>& tasks)
{
    vector<vector<string>> modObj;
    for (auto it=tasks.begin();it!=tasks.end();it++)
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

}
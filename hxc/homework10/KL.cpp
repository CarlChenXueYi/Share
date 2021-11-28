#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

int main()
{
    vector<int> moduleA,moduleB;
    return 0;
}
//1. 计算所有任务的D值 （E-I； E是全部外部成本，I是全部内部成本）
void caculateD(int index,vector<int>& moduleA,vector<int>& moduleB,vector<vector<int>>& commCostMatrix)
{

    return vector<int> Da,Db;

}

//2. A中精选a，B中精选b，返回结果是ai下标，bi下标，以及gain
pair<pair<int,int>,int> selectedElement(vector<bool>& curTask,vector<int>& Da,vector<int>& Db,int curNum,vector<vector<int>>& curCommCostMatrix)
{
    int ai,bi;
    int gain = INT32_MIN;
    for (int i=0;i<curNum;i++)
    {
        for (int j=0;j<curNum;j++)
        {
            if (Da[i]+Db[j]-curCommCostMatrix[i][j] > gain)
            {
                ai = i;
                bi = j;
                gain = Da[i] + Db[j] - curCommCostMatrix[i][j];
            }
        }
    }

    return pair<pair<int,int>,int>{{ai,bi},gain};
}

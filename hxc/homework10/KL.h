#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

void caculateD(vector<bool>&   curTask,
               int             index,
               vector<double>& D,
               vector<int>&    moduleA,
               vector<int>&    moduleB,

               vector<vector<double>>& commCostMatrix);

pair<pair<int, int>, double> selectedElement(vector<bool>&           curTask,
                                             vector<double>&         D,
                                             vector<int>&            moduleA,
                                             vector<int>&            moduleB,
                                             vector<vector<double>>& commCostMatrix);

void KL(int                     taskNum,
        vector<double>&         D,
        vector<int>&            moduleA,
        vector<int>&            moduleB,
        vector<vector<double>>& commCostMatrix);

void   randSelect(vector<int>& moduleA, vector<int>& moduleB, int taskNum);
double sumCommCost(vector<int>& moduleA, vector<int>& moduleB, vector<vector<double>>& commCostMatrix);

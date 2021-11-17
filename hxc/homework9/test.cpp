#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2 * !!(condition)]))

using namespace std;

int main()
{
    vector<vector<int>> matrix = {{1, 2, 3}, {1, 2}, {4}, {6, 5, 4}, {1, 3, 7}};
    for (auto formerIt = matrix.begin(); formerIt != matrix.end(); formerIt++)
    {
        auto laterIt = formerIt + 1;
        while(laterIt != matrix.end())
        {
            if (laterIt->size()<=3)
            {
                for (auto it=laterIt->begin();it!=laterIt->end();it++)
                {
                    formerIt->push_back(*it);
                }   
                matrix.erase(laterIt);
                //laterIt++;
            }
            else
                laterIt++;
            for (auto ait=matrix.begin();ait!=matrix.end();ait++)
            {
                for (auto bit = ait->begin();bit!=ait->end();bit++)
                {
                    cout << *bit << " ";
                }
                cout << endl;
            }
            cout << "fin" << endl;
        } 
    }
}

#include "Task.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

void GetInfo(string inputName, vector<vector<double>>& commCostMatrix, vector<string>& tasks)
{
    string   filePrefix = "/root/Share/hxc/homework10/";
    string   inputFile  = filePrefix + inputName;
    ifstream input;
    cout << inputFile << endl;
    input.open(inputFile, ios::in);
    if (!input.is_open())
    {
        cout << "读取文件失败" << endl;
        return;
    }
    string buf;
    // get task number
    getline(input, buf);
    int taskNum = stoi(buf);


    getline(input, buf);
    stringstream taskNames(buf);
    string taskName;
    while (taskNames >> taskName)
        tasks.push_back(taskName);


    //commCostMatrix.resize(taskNum, vector<int>(taskNum));
    for (int i = 0; i < taskNum; i++)
    {
        vector<double> temp;
        getline(input, buf);
        stringstream taskNamess(buf);
        string taskNamee;
        while (taskNamess >> taskNamee)
        {
            temp.push_back(stod(taskNamee));
        }

        commCostMatrix.push_back(temp);
        // for (int j = 0; j < taskNum; j++)
        //     commCostMatrix[i][j] = buf[j] - '0';
        // task[i][j] = buf[j] - '0';
    }
}


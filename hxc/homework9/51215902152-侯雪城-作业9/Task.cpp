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

void GetInfo(string inputName, vector<vector<int>>& commCostMatrix, vector<string>& tasks)
{
    string   filePrefix = "/root/Share/hxc/homework9/";
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
        std::cout << i << std::endl;
        vector<int> temp;
        getline(input, buf);
        stringstream taskNamess(buf);
        string taskNamee;
        while (taskNamess >> taskNamee)
            temp.push_back(stoi(taskNamee));

        commCostMatrix.push_back(temp);
        // for (int j = 0; j < taskNum; j++)
        //     commCostMatrix[i][j] = buf[j] - '0';
        // task[i][j] = buf[j] - '0';
    }
}
// void GetTask(string inputName, int& taskNum, vector<vector<int>>& task, vector<Task>& myTask)
// {
//     string   filePrefix = "/root/Program/homework/hardware_software/homework8/";
//     string   inputFile  = filePrefix + inputName;
//     ifstream input;

//     input.open(inputFile, ios::in);
//     if (!input.is_open())
//     {
//         cout << "读取文件失败" << endl;
//         return;
//     }

//     string buf;
//     // get task number
//     getline(input, buf);
//     taskNum = stoi(buf);

//     // int  task[taskNum][taskNum];
//     // Task myTask[taskNum];

//     // get task matrix
//     task.resize(taskNum, vector<int>(taskNum));
//     for (int i = 0; i < taskNum; i++)
//     {
//         getline(input, buf);
//         for (int j = 0; j < taskNum; j++)
//             task[i][j] = buf[j] - '0';
//         // task[i][j] = buf[j] - '0';
//     }

//     // get release time
//     myTask.resize(taskNum);
//     getline(input, buf);
//     for (int i = 0; i < taskNum; i++)
//         myTask[i].release_time = buf[i] - '0';

//     // get execution time
//     getline(input, buf);
//     for (int i = 0; i < taskNum; i++)
//     {
//         myTask[i].left_exec_time       = buf[i] - '0';
//         myTask[i].origin_exec_time = buf[i] - '0';
//     }

//     for (int i = 0; i < taskNum; i++)
//     {
//         myTask[i].prior      = 0;
//         myTask[i].seq        = i + 1;
//         myTask[i].out_degree = 0;
//         for (int j = 0; j < taskNum; j++)
//         {
//             if (task[i][j] == 1)
//             {
//                 myTask[i].out_degree++;
//             }
//         }
//     }

//     // count every task's priority value
//     for (int i = taskNum - 1; i >= 0; i--)
//     {
//         int tempPrior = 0;
//         for (int j = 0; j <= taskNum; j++)
//         {
//             if (task[i][j] == 1 && myTask[j].prior > tempPrior)
//                 tempPrior = myTask[j].prior;
//         }
//         myTask[i].prior = myTask[i].left_exec_time + myTask[i].out_degree + tempPrior;
//         myTask[i].prior = myTask[i].origin_exec_time + myTask[i].out_degree + tempPrior;
//     }
// }

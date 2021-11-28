#include<iostream>
#include<vector>
#include<fstream>
#include"KL.hpp"
using namespace std;

int main(){
    ifstream myfile("problem7_2.txt");
    if(!myfile.is_open()){
        cout<<"未能打开文件"<<endl;
        return -1;
    }
    int n;
    myfile>>n;
    vector<string> d(n);
    for(int i=0;i<n;i++){
        myfile>>d[i];
    }
    vector<vector<float>> a(n,vector<float>(n,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            myfile>>a[i][j];
        }
    }
    KL kl(n,n,d,a);
    kl.Dichotomy();
    return 0;
}

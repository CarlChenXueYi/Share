#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>
#include"KL.hpp"
using namespace std;

vector<vector<int>> ret;
void KL_n(int orinum,int n,vector<string> d,vector<vector<float>> a,vector<int>nodes,int k){
    if(k==0) {
        ret.push_back(nodes);
        return;
    }
    KL kl(orinum,n,d,nodes,a);
    kl.Dichotomy();
    k--;
    KL_n(orinum,n>>1,d,a,kl.getA(),k);
    KL_n(orinum,n>>1,d,a,kl.getB(),k);
}

int main(){
    ifstream myfile("problem7_3.txt");
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
    int k;          //k式划分
    int mpk;        //每个划分中的最大个数
    myfile>>k;
    myfile>>mpk;
    int i=1;
    while(k>(1<<i)){
        i++;
    }
    if(n>mpk*(1<<i)){
        cout<<"无法划分！"<<endl;
    }else{
        //补零元素及其通讯代价
        int fn=mpk*(1<<i);
        for(int j=n;j<fn;j++){
            d.push_back(" ");
        }
        for(int j=0;j<n;j++){
            for(int k=n;k<fn;k++){
                a[j].push_back(0);
            }
        }
        for(int j=n;j<fn;j++){
            a.push_back(vector<float>(fn,0));
        }
        vector<int>nodes;
        for(int j=0;j<fn;j++){
            nodes.push_back(j);
        }
        //调用KL_n计算
        KL_n(fn,fn,d,a,nodes,i);
        for(int j=0;j<ret.size();j++){
            for(auto x=ret[j].begin();x<ret[j].end();x++){
                if(d[*x]==" "){
                    ret[j].erase(x);
                    x--;
                }
            }
            if(ret[j].size()==0){
                ret.erase(ret.begin()+j);
            }
        }
        //输出
        for(int j=0;j<ret.size();j++){
            cout<<"{";
            for(auto x:ret[j]){
                cout<<d[x];
                if(x != *(ret[j].end()-1))
                    cout<<",";
            }
            cout<<"}";
        }
        cout<<endl;
        //输出各集合通讯代价
        for(int j=0;j<ret.size();j++){
            for(int k=j+1;k<ret.size();k++){
                cout<<j<<"集合和"<<k<<"集合的通讯代价为";
                int sum=0;
                for(auto x:ret[j]){
                    for(auto y:ret[k]){
                        sum+=a[x][y];
                    }
                }
                cout<<sum<<endl;
            }
        }
    }
    return 0;
}
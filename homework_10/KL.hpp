#ifndef _KL_
#define _KL_
#include<vector>
#include<algorithm>
using namespace std;

class KL{
private:
    int orinum;
    int num;                        //节点个数
    vector<string> name;            //节点名称
    vector<int> nodes;              //节点
    vector<vector<float>> c;        //节点通讯矩阵
    vector<int>A,B;                 //划分后的节点归属
public:
    KL(int ornu,int n,vector<string> d,vector<vector<float>> a):orinum(ornu),num(n),name(d),c(a){     //构造函数
        for(int i=0;i<num;i++){
            nodes.push_back(i);
        }
    }    
    KL(int ornu,int n,vector<string> d,vector<int> no,vector<vector<float>> a)           //构造函数
    :orinum(ornu),num(n),name(d),nodes(no),c(a){}    
    void Dichotomy();                                                           //KL二分类
    void printAB(vector<int>A,vector<int>B);                                    //打印分类的AB
    void calculatecost();                                                       //通讯代价计算
    vector<int> getA() {return A;}
    vector<int> getB() {return B;}
};

typedef struct{
    int a;
    int b;
}selected;

void KL::Dichotomy(){
    //AB初始化
    A=vector<int>(nodes.begin(),nodes.begin()+(num>>1));
    B=vector<int>(nodes.begin()+(num>>1),nodes.end());
    //循环直至G大于0
    int G;
    do{
        
        vector<float> g(num>>1,(int)0x80000000); //对应的交换收益值
        vector<selected> s(num>>1);              //对应的交换元素
        vector<int> tempA(A),tempB(B);           //删除对应元素后的AB
        for(int i=0;i<A.size();i++){             //轮次
            //求D数组值
            vector<float> D(orinum,0);
            for(auto x:tempA){
                float tempE=0,tempI=0;
                for(auto y:tempB){
                    tempE+=c[x][y];
                }
                for(auto y:tempA){
                    tempI+=c[x][y];
                }
                D[x]=tempE-tempI;
            }
            for(auto x:tempB){
                float tempE=0,tempI=0;
                for(auto y:tempA){
                    tempE+=c[x][y];
                }
                for(auto y:tempB){
                    tempI+=c[x][y];
                }
                D[x]=tempE-tempI;
            }
            //找出使收益最大的ai、bi、gi
            for(auto x:tempA){
                for(auto y:tempB){                                              
                    if(D[x]+D[y]-2*c[x][y]>g[i]){
                        g[i]=D[x]+D[y]-2*c[x][y];
                        s[i].a=x;
                        s[i].b=y;
                    }
                }
            }
            //printAB(tempA,tempB);
            //删除ai、bi
            tempA.erase(find(tempA.begin(),tempA.end(),s[i].a));
            tempB.erase(find(tempB.begin(),tempB.end(),s[i].b));
        }
        
        //计算G值
        G=0;
        for(int i=0;i<g.size();i++){
            if(g[i]>0){
                A.erase(find(A.begin(),A.end(),s[i].a));
                B.erase(find(B.begin(),B.end(),s[i].b));
                A.push_back(s[i].b);
                B.push_back(s[i].a);
                G=G+g[i];
            }
        }
        printAB(A,B);
        //cout<<"G="<<G<<endl;
    }while(G>0);
    calculatecost();
}

void KL::calculatecost(){
    float sum=0;
    for(auto x:A){
        for(auto y:B){
            sum+=c[x][y];
        }
    }
    cout<<"通讯代价为"<<sum<<endl;
}

void KL::printAB(vector<int>A,vector<int>B){
    cout<<"{";
    for(int i=0;i<A.size();i++){
        cout<<name[A[i]];
        if(i!=A.size()-1){
            cout<<",";
        }
    }
    cout<<"}";
    cout<<"{";
    for(int i=0;i<B.size();i++){
        cout<<name[B[i]];
        if(i!=B.size()-1){
            cout<<",";
        }
    }
    cout<<"}";
    cout<<endl;
}

#endif
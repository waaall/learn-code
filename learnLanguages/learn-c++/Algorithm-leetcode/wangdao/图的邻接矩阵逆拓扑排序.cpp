#include <stdio.h>
#include <iostream>
#include <string>

//=================================结构定义=================================
#define MaxVertexNum 100
//邻接矩阵法
struct AMGraph{
    std::string VetData[MaxVertexNum];
    bool Edge[MaxVertexNum][MaxVertexNum];
    int vernum;
};

//拓扑排序用栈和队列均可，因为其不关系入度为零的结点的顺序。
struct StackNode{
    int data;
    StackNode *next;
};


//=================================函数声明=================================
bool InitStack(StackNode * S);
bool Push(StackNode * S, int vernum);
bool Pop(StackNode * S, int & vernum);
bool isEmpty(StackNode * S);

bool InitGraph(AMGraph * Gra);
// bool AddVex(AMGraph * Gra, string data);
// bool AddEdge(AMGraph * Gra, int invexnum, int outvexnum);

bool DeleteVex(AMGraph * Gra, int vexnum);

bool InverTopo(AMGraph * Gra);

int OutDegree(AMGraph * Gra, int vexnum);

//=================================常用函数定义=================================
bool InitStack(StackNode * S) {
    StackNode *S = new StackNode;//分配一个头节点
    S->next = NULL;
    return true;
}

bool Push(StackNode * S, int vernum){
    //入站不需要检查
    StackNode *ls = new StackNode; //这个不是临时变量，不放在栈上，用new
    if (ls == NULL)return false;
    ls->data = vernum;
    ls->next = S->next;
    S->next = ls;
    return true;
}

bool Pop(StackNode * S, int &vernum){
    //判断
    if (S->next == NULL)return false;//栈空,这里的条件
    StackNode *q;       //这个是函数内部的临时变量，直接声明即可，只要不是很大，不需要new
    q = S->next;
    S->next = q->next;
    vernum = q->data;
    return true;
}

bool isEmpty(StackNode * S){
    if (S->next == NULL){
        return true;
    }
    else
        return false;
}

bool InitGraph(AMGraph * Gra){
    for (int i = 0; i < MaxVertexNum; ++i)
    {
        Gra->VetData[i] = "";
        for (int j = 0; j < MaxVertexNum; ++j){
            Gra->Edge[i][j] = 0;
        }
    }
    //修改这部分就是修改图的结构（节点和边）
    Gra->vernum = 7;
    Gra->VetData[0] = "准备厨具";
    Gra->VetData[1] = "打鸡蛋";
    Gra->VetData[2] = "下锅炒";
    Gra->VetData[3] = "吃";
    Gra->VetData[4] = "买菜";
    Gra->VetData[5] = "洗番茄";
    Gra->VetData[6] = "切番茄";

    Gra->Edge[0][1] = true;
    Gra->Edge[0][6] = true;
    Gra->Edge[4][1] = true;
    Gra->Edge[4][5] = true;
    Gra->Edge[1][2] = true;
    Gra->Edge[6][2] = true;
    Gra->Edge[2][3] = true;
    Gra->Edge[5][6] = true;

    return true;
}

//返回该顶点的出度
int OutDegree(AMGraph * Gra, int VetId){
    int outdegrees = 0;
    for (int i = 0; i < Gra->vernum; ++i){
        if (Gra->Edge[VetId][i] == true)
            ++outdegrees;
    }
    return outdegrees;
}

//逆拓扑排序
bool InverTopo(AMGraph * Gra){
    //初始化一个栈，用来暂存出度为0的顶点
    StackNode * vetstack = new StackNode;
    // StackNode * vetstack; //这个会引发错误“illegal hardware instruction”，为什么？
    InitStack(vetstack);

    //检查所有顶点，若出度为零，则压入栈
    for (int i = 0; i < Gra->vernum; ++i){
        if (OutDegree(Gra, i) == 0){
            Push(vetstack,i);
            // std::cout << Gra->VetData[i] << "压入栈" << std::endl;//测试代码
        }
    }
    int count = 0;
    while(!isEmpty(vetstack)){
        //将栈顶顶点出栈
        int outid = 0;
        Pop(vetstack,outid);
        std::cout << Gra->VetData[outid] << "出栈" << std::endl;//测试代码
        ++count;
        for (int k = 0; k < Gra->vernum; ++k){
            if (Gra->Edge[k][outid] == true){
                Gra->Edge[k][outid] = false;
                if (OutDegree(Gra, k) == 0){
                    Push(vetstack,k);
                    // std::cout << Gra->VetData[k] << "压入栈" << std::endl;//测试代码
                }
            }
        }
    }
    if (count < Gra->vernum){
        return false;
    }
    else
        return true;
}

//=================================main函数=================================
int main()
{   
    AMGraph * MyGra = new AMGraph;
    InitGraph(MyGra);

    // 这个网站不错：https://www.cs.usfca.edu/~galles/visualization/Algorithms.html
    InverTopo(MyGra);

    return 0;
}


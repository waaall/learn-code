#include <stdio.h>
#include <iostream>
#include <string>

//=================================结构定义=================================
#define MaxVertexNum 100
//邻接矩阵法
struct Graph{
    std::string VetData[MaxVertexNum];
    bool Edge[MaxVertexNum][MaxVertexNum];
    int vernum;
};

struct StackNode{
    int data;
    StackNode *next;
};

//-----邻接表法（顺序+链式存储）
//"顶点"
struct VNode{
    string data; //顶点信息，数据类型不定，string只是一个例子
    ArcNode *first;//第一条边/弧
};
//"边"/"弧"
struct ArcNode{
    int adjvex;    //边/弧指向哪个结点（结点序号）
    struct ArcNode *next; //指向下一条弧的指针
    //InfoType info;  //边权值
};
//用邻接表存储的图
struct ALGraph{
    VNode AdjList[MaxVertexNum];
    int vexnum,arcnum; //从0开始
};

//=================================函数声明=================================
bool InitGraph(Graph * Gra);

bool AddVex(Graph * Gra, string data);
bool InsertVex(Graph * Gra, int vexnum);
bool DeleteVex(Graph * Gra, int vexnum);

bool AddEdge(Graph * Gra, int invexnum, int outvexnum);
bool DeleteEdge(Graph * Gra, int invexnum, int outvexnum);

int FirstNeighbor(Graph * Gra, int invexnum);
int NextNeighbor(Graph * Gra, int invexnum, int recent_num);



//=================================常用函数定义=================================


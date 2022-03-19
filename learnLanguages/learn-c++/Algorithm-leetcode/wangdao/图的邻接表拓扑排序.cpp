#include <stdio.h>
#include <iostream>
#include <string>

//=================================结构定义=================================

//-----邻接表法（顺序+链式存储）
//"顶点"
struct VNode{
    string data; //顶点信息，数据类型不定，string只是一个例子
    ArcNode *firstarc;//第一条边/弧
};
//"边"/"弧"
struct ArcNode{
    int adjvex;    //边/弧指向哪个结点（结点序号）
    struct ArcNode *nextarc; //指向下一条弧的指针
    //InfoType info;  //边权值
};
//用邻接表存储的图
struct ALGraph{
    VNode AdjList[MaxVertexNum];
    int vexnum,arcnum; //从0开始
};

//=================================函数声明=================================
bool InitGraph(AMGraph * Gra);
// bool AddVex(AMGraph * Gra, string data);
// bool AddEdge(AMGraph * Gra, int invexnum, int outvexnum);

bool DeleteVex(AMGraph * Gra, int vexnum);

bool Topo(AMGraph * Gra);

int OutDegree(AMGraph * Gra, int vexnum);

//=================================常用函数定义=================================
bool TopologicalSort (Graph G){
    InitStack(S);
    //初始化栈，存储入度为0的顶点
    for (int i=0;i<G.vexnum;i++){
        if(indegree[i]==0)
            Push(S,i);      //将所有入度为o的顶点进栈
    }
    int count = 0;          //计数，记录当前已经输出的顶点数
    while (!IsEmpty(S)){
        //栈不空，则存在入度为0的顶点
        Pop(S,i);           //栈顶元素出栈
        PrintList[count++]=i;   // 输出顶点i
        //将所有i指向的顶点的入度减1，并且将入度减为0的顶点压入栈S
        for (p=G.vertices[i].firstarc; p; p=p->nextarc){
            v = p->adjvex;
            //入度为0，则入栈
            if(! (--indegree[v]))
                Push (S,v):
        }
    }
    if (count<G. vexnum)
        return false; //有回路
    else
        return true; //拓扑排序成功
}

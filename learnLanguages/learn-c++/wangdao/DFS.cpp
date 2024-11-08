#include <stdio.h>
#include <iostream>
#include <string>

//=================================结构定义=================================
#define MaxVertexNum 100
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
bool InitGraph(AMGraph * Gra);

int FirstNeighbor(Graph * Gra, int invexnum);
int NextNeighbor(Graph * Gra, int invexnum, int recent_num);

//=================================常用函数定义=================================
bool visited[VertexNum]; //设置visited数组，避免重复visit
//为了避免非连通图落下顶点
void DFSTraverse(Graph * Gra){
    for (int i = 0; i < Gra.vexnum; ++i)
        visited[i] = false;
    InitQueue(Q);
    for (int i = 0; i < Gra.vexnum; ++i)
        if(!visited[i])
            DFS(Gra,i);
}
// 这部分是深度优先遍历的核心：不同于BFS用队列循环，DFS用递归来实现（同树的先序遍历）
void DFS(Graph Gra, int vexnum){
    visit(vexnum);
    visited[vexnum] = true;

    for (w=FirstNeighbor(Gra,vexnum); w>=0; w=NextNeighbor(Gra,vexnum,w)){
        if (!visited[w]){
            DFS(Gra,w);
        }
    }
}



//链表及基本操作
#include <iostream>

//=============================================结构定义=============================================
struct LNode
{
    int data;
    struct LNode *next;
};

struct DNode
{
    int data;
    struct DNode *prior, *next;
};

//=============================================函数声明=============================================
bool InitLinkList(LNode * & L);
bool InsertList(LNode * & L, int i, int el);
LNode * GetElem(LNode * & L, int i);

//=============================================常用函数定义=============================================
//初始化单链表
bool InitLinkList(LNode * & L){
    LNode * L = new LNode;

    if (L == NULL){
        return false;
    }

    L->next = NULL;
    return true;
}

//单链表查找
LNode * GetElem(LNode * & L, int i){
    LNode *p;
    int j = 0;
    p = L;
    while(p!=NULL && j < i){
        p = p->next;
        j++;
    }
    return p;
}

//单链表节点插入
bool InsertList(LNode * & L, int i, int el){
    LNode *p;
    int j = 0;
    p = L;
    while (p!=NULL && j < i-1){
        p=p->next;
        j++;
    }
    LNode *s = new LNode;
    s->data = el;
    s->next = p->next;
    p->next = s;

    return true;
}

//=============================================main函数=============================================
int main()
{   
    int * a = {1,2,3,4}
    return 0;
}


//=============================================自定义函数=============================================
LNode * 合并两单链表(LNode * LA,LNode * LB){
    LNode * LS;
    InitLinkList(LS);

    LNode *p = LA;
    LNode *q = LB;

    int i,j = 0;
    while (*p != NULL && *q!= NULL){

        p=p->next;
        q=q->next
    }
    return LS;
}



//两个整数的除法在c++中舍去余数，结果依然是整数
void 测试除法(){
    int length = 7;
    std::cout << length / 2; 
}

//要求时间复杂度为n，空间复杂度为1
void 删除顺序表某值(Sqlist & L, int x){

    int k = 0; //记录不等于x的元素个数
    for (int i = 0; i < L.length; ++i)
    {
        if (L->data[i])
        {
            /* code */
        }
    }
}


//单链表
// void LinkList()
// {
//     typedef struct LNode
//     {
//         int data;
//         struct LNode *next;
//     }LNode, *LinkList;
    

//     int x; //插入数据
//     scanf("%d", &x); //
//     // using std::cin;

//     LNode * mylist = new LNode; //头节点地址表示链表
//     LNode * tail = new LNode; //这个指针一直指向尾部节点

    // while(x != 9999){
    //     LNode * mybala = new LNode;

    //     mytry->data = x;
    //     tail->next= mytry;

    //     tail = mytry;
    //     delete mytry;
    // }

//     tail->next = NULL;
// }

//双链表
// void DoubleList()
// {
//     typedef struct DNode
//     {
//         int data;
//         struct DNode *prior, *next;
//     };DNode, *DLinkList;
// }

// bool InitDLinkList(DLinkList $L){
//     DNode *L = new DNode;
    
//     if (L == NULL){
//         return false;
//     }

//     L->prior = NULL;
//     L->next = NULL;
//     return true;
// }

// void testDLinkList(){
//     DLinkList L;
//     InitDLinkList(L);
// }

//循环单链表


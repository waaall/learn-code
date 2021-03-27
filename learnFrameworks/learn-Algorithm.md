# 算法与数据结构



有两个问题至关重要：



* **算法和数据结构有什么关系？**
* **这两个东西与编译器、操作系统、计算机网络又是什么关系？**



很明显它们并不是并列关系，其中的复杂关系让我娓娓道来。



> 算法为什么要和数据结构一起讲？

算法，简单可以理解为“计算方法”，但这个计算可不是狭义的数学运算，而是包括了将现实问题抽象成数学模型的思想，但是归根结底是将问题转化为数学计算。这不是计算机的专属，但在计算机领域，特殊的是需要将数学计算进一步简化为二进制的加减乘除。

那？这与数据结构有什么关系？计算方法，当然要针对计算的数据，而方法和数据其实是不可分的，甚至可以说，方法的诞生伴随着数据结构的诞生------比如人类的十进制加减法，对应的就是0-9这十个数字。没有人可以将十进制加减法和0-9这十个数字脱离开（有严谨的同学可能会说，我用一到十这十个汉子，这本质还是这十个符号，用什么都可以，只有这十个符号作为数据，才可以进行加减法）。

而同样的，怎样组织定义数据，往往和怎么处理计算这些数据是密不可分的，甚至这两个过程历史上本就是一个过程。

> 要是这么说，所有都有关系，没有编译器能有操作系统吗，没有算法和数据结构能有编译器吗？



## 线性表





### 顺序表





### 单链表



#### 尾插法建立单链表



```c++
//定义一个单链表
typedef struct LNode{
    ElemType data;
    struct LNode *next;
}LNode, *LinkList;



// 尾插法
LinkList list_TailInsert(LinkList &L){
    int x;
    LinkList L = new LinkList;
	LNode *s, *r = L;
    scanf("%d", &x);
    while(x!=9999){
        LNode *s = new LNode;
        s->data = x;
        r->next = s;
        r = s;
        scanf("%d", &x);
    }
    r->next = NULL;
    return L;
}
```



## 串







## 树







## 代码示例

定义的一些链表、顺序表等数据结构，及其常用的操作函数，最后的一些函数是稍微复杂点的操作。



```c++
#include <iostream>

//===============================结构定义===============================
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

//===============================函数声明===============================
bool InitLinkList(LNode * & L);
bool InsertList(LNode * & L, int i, int el);
LNode * GetElem(LNode * & L, int i);

//===============================常用函数定义===============================
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

//===============================main函数===============================
int main()
{   


    return 0;
}


//===============================自定义函数===============================
//之所以函数名写中文是因为一眼看得出解决哪类问题

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
```


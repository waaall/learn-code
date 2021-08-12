# 算法与数据结构

[toc]



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



#### 递归删除无头节点单链表某值

```c++ 
//删除单链表某值
void 删除单链表某值(LinkList * & L, ElemType x) {
    LNode *p = (LNode *)malloc(sizeof(LNode));
    if (L == NULL) {
        return;
    }
    if (L->data == x) {
        p = L;
        L = L->next;  //精髓所在
        free(p);
        删除单链表某值(L, x);
    }
    else {
        删除单链表某值(L->next, x);
    }
}
```

![729AC06F-BBDC-4576-8875-472EE868ABDE_1_105_c](learn-Algorithm.assets/danlianbiao.jpeg)

上图中没说出事情的全部，我们应该注意到上述代码中，接受的参数是`LinkList *&L`，这点非常关键！

因为我们传入的这个地址，如果类型为`LinkList * L`，那么传入的则是`L->next`的拷贝，则该链表就会断掉。

`LinkList *` 表示这个参数是一个**地址**，而`&`表示，L是这个**地址**的引用，而传递这个**地址**时，不能直接拷贝这个地址，而是去存放这个**地址**的地址寻址！（**加粗**的“地址”是指的上面x1旁边的2，而本句话最后为加粗的地址是指的上图中的1）。



### 栈



#### 出栈合法性



![出栈合法性](learn-Algorithm.assets/出栈合法性.png)







## 串



### 定义





### 朴素匹配算法





### KMP匹配算法



![Screen Shot 2021-04-18 at 10.19.01 AM](learn-Algorithm.assets/Screen Shot 2021-04-18 at 10.19.01 AM.png)

### KMP算法优化



![Screen Shot 2021-04-18 at 10.20.32 AM](learn-Algorithm.assets/Screen Shot 2021-04-18 at 10.20.32 AM.png)









## 树





### 二叉树



![Screen Shot 2021-04-19 at 10.50.41 AM](learn-Algorithm.assets/Screen Shot 2021-04-19 at 10.50.41 AM.png)



#### 二叉树的遍历

* 先序遍历

**递归**调用来实现，二叉树的遍历：

![Screen Shot 2021-04-19 at 10.58.32 AM](learn-Algorithm.assets/Screen Shot 2021-04-19 at 10.58.32 AM.png)



* 层序遍历



![Screen Shot 2021-04-19 at 11.51.22 AM](learn-Algorithm.assets/Screen Shot 2021-04-19 at 11.51.22 AM.png)





#### 二叉树的应用

##### 算数表达式：

<img src="learn-Algorithm.assets/Screen Shot 2021-04-19 at 11.02.03 AM.png" alt="Screen Shot 2021-04-19 at 11.02.03 AM" style="zoom: 25%;" />

```c
先序遍历：-+a*b-cd/ef	//前缀表达式
中序遍历：a+b*c-d-e/f	//中缀表达式（需要加界限符）
后序遍历：abcd-*+ef/-	//后缀表达式
```

计算机的输入只会是中缀表达式（因为是人输入的计算式）。所以问题1是怎么将中缀表达式转换为表达式树，问题2是有了表达式树怎么进行计算。

* [调度场算法](https://zh.wikipedia.org/wiki/调度场算法)

* [逆波兰表示法](https://zh.wikipedia.org/wiki/逆波兰表示法#C++程序实现)

调度场算法是将中缀表达式转换为后缀表达式，而后者又称逆波兰表示法，是计算机进行数学计算的高效方式。然而在实际中，往往会讲中缀表达式转化为表达式树，然后其便利序列就是后缀、前缀、中缀表达式。







##### 求树的深度：

![Screen Shot 2021-04-19 at 11.09.28 AM](learn-Algorithm.assets/Screen Shot 2021-04-19 at 11.09.28 AM.png)

#### 线索二叉树



![Screen Shot 2021-04-22 at 9.45.32 PM](learn-Algorithm.assets/Screen Shot 2021-04-22 at 9.45.32 PM.png)



**中序线索化代码过程：**

![Screen Shot 2021-04-22 at 9.46.08 PM](learn-Algorithm.assets/Screen Shot 2021-04-22 at 9.46.08 PM.png)



上述代码会建立一个pre的全局变量：



**先序线索化代码过程：**

我们需要注意到这样一个问题：如果访问到没有左子树的节点，将该节点的左子树节点置为其前驱节点。然后若此时再访问这个节点的左子树就会有问题。

**Question：为何中、后序线索化就没有这种问题？**

首先右子树总不会存在该问题，因为其若为NULL，则被替换为后继节点，也就是访问下个节点时才会更改；其次在中后序线索化中，先访问左子树，所以总会先“解决”那个左子树为NULL的节点，所以不会出现这种问题。

![Screen Shot 2021-04-22 at 10.15.49 PM](learn-Algorithm.assets/Screen Shot 2021-04-22 at 10.15.49 PM.png)



**中序线索二叉树找节点的前驱、后继：**

![Screen Shot 2021-04-23 at 10.37.03 AM](learn-Algorithm.assets/中序线索二叉树找后继.png)

```c
//------------------中序线索二叉树找中序后继------------------
//找到以P为根的子树中，第一个被中序遍历的结点
ThreadNode *FirstNode(ThreadNode *p){
    //循环找到最左下结点（不一定是叶结点）
    while(0==p->ltag){
        p=p->lchild;
    }
    return p;
}

//在中序线索二叉树中找到结点p的后继结点
ThreadNode *NextNode(ThreadNode *p){
    //在右子树中最左下结点
    if(0==p->rtag)return FirstNode(p->rchild);
    else return p->rchild;
}

//对中序线索二叉树进行中序遍历（利用线索实现的非递归算法），空间复杂度为O(1)；
void InOrder(ThreadNode *T){
    for (ThreadNode *p = FirstNode(T);  p!=NULL ; p=NextNode(p)) {
        visit(p);
    }
}
```

##### 总结



![Screen Shot 2021-04-26 at 10.04.24 AM](learn-Algorithm.assets/Screen Shot 2021-04-26 at 10.04.24 AM.png)

### 树的存储结构

**Question：为啥会有树的存储结构？明明二叉树都讲过了呀？**

因为二叉树最多可以有两个子树节点，所以sturct是确定的，而一般意义的树是可以有任意多的子树节点的，所以这就让节点struct表述很困难。

![Screen Shot 2021-04-26 at 11.28.18 AM](learn-Algorithm.assets/Screen Shot 2021-04-26 at 11.28.18 AM.png)



#### 树、森林的遍历









### 二叉排序（搜索）树BST

知道了中序遍历等方法， 再结合之前提到的二叉排序树就可以发现它的好处———高效查找、排序。

* BST的中序遍历结果是一个递增序列



#### BST查找





#### BST插入





#### BST删除











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

//删除单链表某值
void 删除单链表某值(LinkList *&L, ElemType x) {
    LNode *p = (LNode *)malloc(sizeof(LNode));
    if (L == NULL) {
        return;
    }
    if (L->data == x) {
        p = L;
        L = L->next;  //精髓所在
        free(p);
        删除单链表某值(L, x);
    }
    else {
        删除单链表某值(L->next, x);
    }
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


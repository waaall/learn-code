#include <stdio.h>
#include <iostream>
#include <string>

//=================================基本定义=================================
struct LNode{
    int data;
    LNode *next;
};

//初始化
void InitLinkList(LNode* & L){
    int a = 1;
}
//=================================常考函数=================================

//找中间节点
LNode* Find_Mid(LNode* L){
    LNode* a;
    LNode* b = new LNode;
    // 将a、b都指向第一个数据节点(这是带头节点的单链表)
    a = L->next;
    b = L->next;
    //a每次走两步，b走一步，a到头结束，返回b
    while(a->next == NULL){
        a = a->next->next;
        if (a == NULL)
            return b; //这个是[n/2]
        b = b->next;
    }
    return b;   //这个正好是第二个
}

//找倒数第k个节点
int Print_Reverse_k(LNode* L, int k){
    LNode* a = L;
    LNode* b = L;
    //先让a移动k次，如果链表长度小于k，则返回0
    for (int i = 0; i < k; ++i){
        a = a->next;
        if (a==NULL) return 0;
    }
    //a、b同时移动，a到头时，b就是倒数第k个，print出来，并返回1
    while(a!=NULL){
        a = a->next;
        b = b->next;
    }
    printf("%d\n", b->data);
    return 1;       
}

//逆置
void Reverse(LNode* & L){
    
}


//=================================main函数=================================
int main()
{

    return 0;
}

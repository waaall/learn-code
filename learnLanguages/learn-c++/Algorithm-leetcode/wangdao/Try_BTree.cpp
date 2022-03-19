//B树（B Trees）
#include <stdio.h>
#include <string>
#include <iostream>

#define MaxNodeNum = 4;

struct BTNode{
    int data[MaxNodeNum];
    struct BTNode *childs[MaxNodeNum+1];
};

//查找
BTNode * BST_Search(BTNode * T, int data[4]) {
    while (T != NULL && data[4] != T->data[4]) {    //若数空或等于根结点的值，则结束循环
        if (data[4] < T->data[4])
            T = T->lchild;//小于，则在左子树上找
        else T = T->rchild;//大于，则在右子树上找
    }
    return T;
}

//查找（递归实现）
BTNode * BTSearch(BTNode * T, int data[4]) {
    if (T == NULL)
        return NULL;//查找失败
    if (data[4] == T->data[4])
        return T;//查找成功
    else if (data[4] < T->data[4])
        return BTSearch(T->lchild, data[4]);//在左子树中查找
    else return BTSearch(T->rchild, data[4]);//在右子树中查找
}

//插入
int BTInsert(BTNode * &T, int k) {
    if (T == NULL) {
        T = (BTNode *) malloc(sizeof(BTNode));
        T->data[4] = k;
        T->lchild = T->rchild = NULL;
        return 1;//插入成功，返回1
    } else if (k == T->data[4])
        return 0;//树中存在相同关键字的结点，插入失败
    else if (k < T->data[4])
        return BTInsert(T->lchild, k);//插入到左子树
    else return BTInsert(T->rchild, k);//插入到右子树
}
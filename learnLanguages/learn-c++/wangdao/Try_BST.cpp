//二叉排序树（又叫二叉查找树，Binary Search Trees）
#include <stdio.h>
#include <stdlib.h>

typedef struct BSTNode {
    int key;
    struct BSTNode *lchild, *rchild;
} BSTNode, *BSTree;


//查找
//在二叉排序书中查找值为key的结点
BSTNode *BST_Search(BSTree T, int key) {
    while (T != NULL && key != T->key) {    //若数空或等于根结点的值，则结束循环
        if (key < T->key)
            T = T->lchild;//小于，则在左子树上找
        else T = T->rchild;//大于，则在右子树上找
    }
    return T;
}

//在二叉排序树中查找值为key的结点（递归实现）
BSTNode *BSTSearch(BSTree T, int key) {
    if (T == NULL)
        return NULL;//查找失败
    if (key == T->key)
        return T;//查找成功
    else if (key < T->key)
        return BSTSearch(T->lchild, key);//在左子树中查找
    else return BSTSearch(T->rchild, key);//在右子树中查找
}

//插入
//在二叉排序树插入关键字为k的新结点（递归实现）
int BSTInsert(BSTree &T, int k) {
    if (T == NULL) {
        T = (BSTree) malloc(sizeof(BSTNode));
        T->key = k;
        T->lchild = T->rchild = NULL;
        return 1;//插入成功，返回1
    } else if (k == T->key)
        return 0;//树中存在相同关键字的结点，插入失败
    else if (k < T->key)
        return BSTInsert(T->lchild, k);//插入到左子树
    else return BSTInsert(T->rchild, k);//插入到右子树
}

//按照 str[] 中的关键字序列建立二叉排序树
void Creat_BST(BSTree &T,int str[],int n){
    T=NULL;//初始化为空
    int i=0;
    while(i<n){ //依次将每个关键字插入到二叉排序树中
        BSTInsert(T,str[i]);
        i++;
    }
}
//不同关键字序列可能得到同款二叉排序树，而可能得到不同款二叉排序树

int main() {
    return 0;
}
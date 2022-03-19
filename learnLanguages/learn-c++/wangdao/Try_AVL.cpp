//平衡二叉树（AVL Tree）
#include <stdio.h>

struct AVLNode {
    int key;
    int balance;
    struct AVLNode *lchild, *rchild;
};


//查找
AVLNode * AVL_Search(AVLNode * T, int key) {
    while (T != NULL && key != T->key) {    //若数空或等于根结点的值，则结束循环
        if (key < T->key)
            T = T->lchild;//小于，则在左子树上找
        else T = T->rchild;//大于，则在右子树上找
    }
    return T;
}

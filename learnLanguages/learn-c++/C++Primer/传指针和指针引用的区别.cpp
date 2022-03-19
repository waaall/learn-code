#include <iostream>

using namespace std;

// 传入指针也就意味着改指针指向的数据在函数外也生效；
// 但是若改这个地址本身，由于这个地址是个copy，地址的修改无效；
// 若要改地址，可以传指针的引用，也就是 int *& p1
void swap(int * p1, int * p2){
    int * temp = p1;
    p1 = p2;
    p2 = temp;

    cout << "==========执行传指针时：==========" << endl;
    cout << "a:" << *p1 << endl << "b:" << *p2 << endl;
    cout << "a的地址：" << p1 << endl << "b的地址：" << p2 << endl;
}

void swapChangeAddress(int *& p1,int *& p2){ //这是传的指针的引用，换句话说，p1是个地址，这是传的p1的地址（地址的地址）
    int * temp = p1;
    p1 = p2;
    p2 = temp;

    cout << "==========执行传指针引用时：==========" << endl;
    cout << "a:" << *p1 << endl << "b:" << *p2 << endl;
    cout << "a的地址：" << p1 << endl << "b的地址：" << p2 << endl;
}

void swapr(int & a, int & b)    // use references
{
    int temp;
    temp = a;       // use a, b for values of variables
    a = b;
    b = temp;
}
void swapp(int * p, int * q)    // use pointers
{
    int temp;
    temp = *p;      // use *p, *q for values of variables
    *p = *q;
    *q = temp;
}
void swapv(int a, int b)        // try using values
{
    int temp;
    temp = a;      // use a, b for values of variables
    a = b;
    b = temp;
}


int main(int argc, char const *argv[])
{
    int a = 1, b = 3;

    // 这是对指针的初始化
    int * p1 = &a;
    int * p2 = &b;
    cout << "通过交换地址来修改指针指向的内容，函数传入两指针" << endl;
    
    cout << "=========执行前：==========" << endl;
    cout << "a:" << *p1 << endl << "b:" << *p2 << endl;
    cout << "a的地址：" << p1 << endl << "b的地址：" << p2 << endl;
    
    swap(p1, p2);

    cout << "==========执行传指针后：==========" << endl;
    cout << "a:" << *p1 << endl << "b:" << *p2 << endl;
    cout << "a的地址：" << p1 << endl << "b的地址：" << p2 << endl;

    swapChangeAddress(p1, p2); 
    cout << "==========执行传指针的引用后：==========" << endl;
    cout << "a:" << *p1 << endl << "b:" << *p2 << endl;
    cout << "a的地址：" << p1 << endl << "b的地址：" << p2 << endl;

    return 0;
}
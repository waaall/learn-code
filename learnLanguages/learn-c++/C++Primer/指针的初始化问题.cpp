#include <iostream>

using namespace std;

void swap(int * p1, int * p2){
    cout << "a的地址：" << p1 << endl << "b的地址：" << p2 << endl;
    
    int * temp = p1;
    p1 = p2;
    p2 = temp;

    cout << "a:" << *p1 << endl << "b:" << *p2 << endl;
    cout << "a的地址：" << p1 << endl << "b的地址：" << p2 << endl;
}

int main(int argc, char const *argv[])
{
    int a = 1, b = 3;

    // 这是对指针的初始化
    int * p1 = &a;
    int * p2 = &b;

    // 下面这几行代码就是错误，因为没有对a（地址）初始化，就进行了访问；
    // 这是编译器申请了一个地址存放a（这个地址），
    // 但是a本身的值（虽然也是一个地址）没有被赋值；
    // int * a;
    // *a = 1; // 这是：先访问a所在的地址，拿到a在以a的值为地址访问，这时出了问题。

    // swap(p1,p2);

    // new 返回一个指针
    int * pt = new int;
    * pt = 5;
    cout << "pt(地址):" << pt << endl << "pt(值):" << *pt << endl;

    return 0;
}
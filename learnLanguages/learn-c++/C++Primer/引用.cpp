#include <iostream>
#include <string>

int main(int argc, char const *argv[])
{
    using namespace std;
    
    int rats;
    rat = 1
    int & rodents = rats; //与下句等同但是，很明显，引用是一个值，而const p 是一个地址！
    int* const pr = &rats; //int* 这是一个指针，指向const pr，所以pr是常量，地址（指针本身）可以变

    
    
    int a;
    const int* p = &a; //const在*的左边，这是一个不能变的指针，所以地址不能变，p不能变，*p能变

    int b;
    const int & ref = b;


    return 0;
}
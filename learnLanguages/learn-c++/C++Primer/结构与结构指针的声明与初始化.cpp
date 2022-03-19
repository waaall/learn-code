#include <iostream>
using namespace std;

#define MaxSize 100

struct Dongtai{
    int * data;
    int maxsize;
    int length;
};

struct Jingtai{
    int data[MaxSize];
    int length;
};

int main(int argc, char const *argv[])
{
    // 这样声明，这个myjt就在“堆上”，也就是需要在函数指令时需要访存
    Jingtai * myjt;
    myjt = new Jingtai;

    // 这样声明，这个myjt2就在函数栈中，调用myjt2时不需要访存。
    Jingtai myjt2;

    // Dongtai mydt;
    for (int i = 0; i < 10; ++i)
    {
        cout << myjt->data[i] << endl;
    }

    return 0;
}
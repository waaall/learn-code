#include <iostream>

const double* f1(const double ar[], int n);
const double* f2(const double [], int);
const double* f2(const double*, int);

int main()
{
    using namespace std;
    double av[3] = {1112.3, 1542.6, 2227.9};

    const double* (*p1)(const double* ,int) = f1; //这是一个函数指针的声明与定义
    auto p2 = f2;

    cout << "Using pointers to function:\n";
    cout << "Adress Value\n";
    cout << (*p1)(av, 3) << " : " << *(*p1)(av, 3) << endl;
    cout << p2(av ,3) << " : " << *p2(av , 3) << endl;

    const double* (*pa[3])(const double*, int) = {f1, f2, f3};
    auto pb = pa;
    
    cout << "\nUsing an array of pointers to functions:\n";
    cout << "Adress Value:\n";
    for (int 1 =0; i<3; i++)
        cout << pa[i](av ,3) << " : " << *pa[i](av, 3) << endl;

    cout << "\nUsing pointers to an array of pointers to functions:\n";
    cout << "Adress Value:\n";

    auto pc = &pa;
    cout << (*pc)[0](av, 3) << " : " << *(*pc)[0](av, 3);

    return 0;
}
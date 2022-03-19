#include <iostream>

int main()
{
    using namespace std;
    const char * wail = "ululate";    // wail points to string

    int a = 12;
    int * p = &a;

    cout << wail << endl;    //cout 重载了 指向char类型的地址，直接将其显示为字符串本身
    
    //下面指令结果：0x10539df50
    cout << (int *) wail << endl;   //强制类型转换为int*，由于cout不重载int指针，所以能显示地址

    //下面指令结果：0x10539df50
    printf("%p", wail);      

    //下面指令结果：ululate
    printf("%s", wail);      

    //下面指令结果：87678800
    printf("%d", wail);      //warning: format specifies type 'int' but the argument has type 'const char *'
    
    return 0;
}
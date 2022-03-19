#include <iostream>
#include <string>

int main()
{
    using namespace std;
     
    printf("%s\n","hello world!, this is in c++");      //图上少了\n,这个代表输入完这个要换行.

    for (int i = 0; i<5; i++)   //这里没有花括号，新标准下也是合法的
        cout << "c++ knows loops.\n";
    cout << "C++ knows when to stop.\n";
    

    return 0;
}

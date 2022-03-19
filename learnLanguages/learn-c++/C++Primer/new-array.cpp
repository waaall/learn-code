#include <iostream>

int main()
{
    using namespace std;
    int a;
    cout << "Type the lenth of a list: ";
    cin >> a;

    //用new在堆（heap）/自由存储区（free）创建动态数组（对于上述变量和数组的创建，没有用new的，都是在‘栈’中）
    int* psome = new int[a];  // 这是常规数组做不到的

    //输入数组元素
    int i;
    for (i = 0; i<a; i++)
    {   //这里没有花括号，新标准下也是合法的
        cout << "Type the " << i+1 << "th element of the list: ";
        cin >> *(psome + i);
    };

    //输出数组元素
    cout << endl << "The list is [ ";
    for (i = 0; i<a; i++)
        cout << psome[i] << " "; //c++都是用指针处理数组，所以声明了一个指针指向这个数组时，同样可以作为数组名使用
    cout << "]\n";

    delete [] psome;
    
    return 0;
}
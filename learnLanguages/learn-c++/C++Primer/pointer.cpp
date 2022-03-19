#include <iostream>
#include <string>

int Baby(const int* a);

int main()
{
    using namespace std;
 
    // 整数与指针
    cout << endl << "This part is 数与指针" << endl;
    int a = 6;
    int* p;
    p = &a;
    cout << "Adress of vaiable a is " << p << endl;
    cout << "a is equal to " << *p;

    // 用new在堆（heap）/自由存储区（free store）申请内存来储存数
    int* pt = new int;  //new and delete to apply and relese a memory block
    *pt = 6;
    cout << "Adress of the int data '6' is " << pt << endl;
    cout << "This is not a vaiable, but it do have a memory block. " << endl;
    delete pt ;



    // 数组与指针 数组名直接可以当指针用（有时候不可以）
    cout << endl << "This part is 数组与指针" << endl;
    int Array[7] = {5, 3, 54, 65, 77, 88, 99};
    int* pl = Array;
    cout << pl << " is Adress of the Array. " << endl;
    cout << *pl << endl << *(pl+4) << endl;
    cout << Array << endl << pl << endl << pl+1 << endl << pl << endl;
    cout << Array+1 << endl << Array[1] << endl;
    cout << "*Array = " << *Array << endl;

    //指针数组？所以这时把int*看作一个整体就很容易理解了，这就是一个包含两个指针的数组
    int* pointers[2];
    pointers[0] = pl;
    pointers[1] = p;



    //用new在堆（heap）/自由存储区（free）创建动态数组（对于上述变量和数组的创建，没有用new的，都是在‘栈’中）
    int* psome = new int[10];
    cout << endl << psome[1] << endl; //c++都是用指针处理数组，所以声明了一个指针指向这个数组时，同样可以作为数组名使用
    delete [] psome;

    //数组名与指针不等价的情况
    psome = psome +1;  //psome是指针，所以这是合法的，但是数组名不可以。而且它在+1之后就不再指向

    return 0;
}


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
    int * pz = new int[100];
    int * pl = pz;

    // 赋值/初始化
    for (int i = 0; i < 10; ++i){
        pz[i] = 5;
    }

    cout << "这是pl访问：\n";
    for (int i = 0; i < 10; ++i){
        cout <<  pl[i] << endl;
    }

    // pl不是new申请的，且更不是一个数组指针，为啥可以delete？且再用pz访问，数据已经“脏”了
    // 另外，delete pl 和 delete [] pl 和 delete [] pz 是一样的; 而 delete pz编译不通过。
    delete [] pl;
    
    // 我猜测是delete这个函数写的有问题，只接受一个地址就可以，很容易出现这种不该delete的数据可以用delete 
    // 很容易证实我的猜测，就是我申请一个栈上的数组，而用一个指针指向它，看看能不能delete，也就是下面这一段
    // int pz[100];
    // int * pl = pz; 

    // // 赋值/初始化
    // for (int i = 0; i < 10; ++i){
    //     pz[i] = 5;
    // }
    // delete [] pl; //error：pointer being freed was not allocated

    cout << "这是delete [] pl后，pz访问：\n";
    for (int i = 0; i < 10; ++i){
        cout << pz[i] << endl;
    }

    // // delete pz不行，需要delete [] pz; 且效果与上面delete [] pl,然后用pz访问数组一样（脏数据）
    // delete [] pz;

    // cout << "这是delete [] pz后，pl访问：\n";
    // for (int i = 0; i < 10; ++i){
    //     cout << pl[i] << endl;
    // }

    return 0;
}
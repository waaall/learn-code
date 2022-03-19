//函数模版和函数重载都为了实现：用同一函数名（功能也类似）实现不同参数的调用（实际上还是多个函数，但调用时看起来是一个）
#include <iostream>


using namespace std;
//C++函数模版两种定义方式 
//template < typename T>  或  template <class T> 本质是一样的
 
template <typename T1> 
T1 check_max(T1 x, T1 y);

template <class T>
T  print_value(T x, T y);
 
 
 
//C++重载
 int add(int x,int y);
 int add(int x,int y,int z);
 string add(string str , string str1 , string str2);
 
int main(void)
{
    int x = 33;
    int y = 44;
    long l1 = 333, l2 = 444;
    float f1 = 3.14, f2 = 3.15926;
 
    //系统会自动识别类型 T1为int类型
    cout << "max(x, y) = " << check_max(x, y) << endl;
    //系统会自动识别类型 T1为long类型
    cout << "max(x, y) = " << check_max(l1, l2) << endl;
    //系统会自动识别类型 T1为float类型
    cout << "max(x, y) = " << check_max(f1, f2) << endl;
    cout << "==========================================" << endl ;
    print_value(x,y);
    print_value(l1,l2);
    print_value(f1,f2);
    
    
    //重载的使用
    cout << "==========================================" << endl ;
    cout << add(x,y) << endl ; 
    cout << add(x,y,x) << endl ;
    cout << add("hello"," ","world") << endl ;
    return 0;
}
 
template <typename T1>  //对于同样的类型
T1 check_max(T1 x, T1 y)
{
    return x > y ? x : y;
}
 
template <typename T>
T  print_value(T x, T y)
{
    cout <<"x:"<< x << endl ; 
    cout <<"y:"<< y << endl ;
}
 
int add(int x,int y)
{
    return x+y ;
}
int add(int x,int y,int z)  //这里我们就可以看到，函数重载可以适用于参数个数不一样的情况，而对此函数模版就无能为力了
{
    return x+y+z ;
}
string add(string str , string str1 , string str2)
{
    return str+str1+str2 ;
}
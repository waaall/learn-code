#include <iostream>

#ifndef STRINGBAD
#define STRINGBAD 

class StringBad
{
private:
    char *str;
    int len;
    static int num_strings; //所有类对象共享同一个静态成员。这是为“学习“使用的，目的看看对象什么时候创建，又什么时候被“销毁”

public:
    StringBad(); //默认构造函数
    StringBad(const char *s); //创建对象的构造函数
    StringBad(const String & s); //复制构造函数，指针复制时会有问题，所以不用默认自动生成的
    ~StringBad(); //析构函数，有new这里需要delete
    StringBad & operator=(const StringBad & st); //重载赋值运算符，同样指针复制时会有问题，所以不用默认自动生成的

    //friend function
    friend std::ostream & operator<<(std::ostream & os, const StringBad & st); //使用友元函数重载<<运算符

};

#endif
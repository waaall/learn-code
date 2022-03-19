#ifndef FRIENDTIME_H
#define FRIENDTIME_H

#include <iostream>

class Time
{
private:
    int hours;
    int minutes;
    
public:
    Time();
    Time(int h, int m = 0);
    // ~Time();
    void AddMin(int m);
    void AddHr(int h);
    void Reset(int h = 0, int m = 0);
    void Show() const;
    Time operator+(const Time & t) const;
    Time operator-(const Time & t) const;//运算双方都是这种类对象的话，像加减，就不需要友元函数
    //下面这是设定友元函数来实现“乘法符号重载后的交换性”
    Time operator*(double mult) const;
    friend Time operator*(double m, const Time & t) //这不会有错误吗？两个operator*？
    {return t * m;}

    friend std::ostream & operator<<(std::ostream & os, const Time & t);

};


#endif
#include <iostream>
#include "mytime00.h"

Time::Time()
{
    hours = minutes = 0;
}

Time:Time(int h, int m)
{
    hours = h;
    minutes = m;
}

void Time::AddMin(int m)
{
    minutes += m;
    hours +=minutes / 60;
    minutes %= 60;
}

void Time::AddHr(int h)
{
    hours += h;
}

void Time::Reset(int h, int m)
{
    hours = h;
    minutes = m;
}

Time Time::operator+(const Time & t) const //函数内部创建的变量如果需要返回就不能返回引用，此处的sum就是这样
{
    Time sum; //这个sum是小写，和声明中的不一样，但是这并不需要一样，这点一定要注意
    sum.minutes = minutes + t.minutes;
    sum.hours = hours + sum.minutes / 60;
    sum.minutes %= 60;
    return sum;
}

void Time::Show() const
{
    std::cout << hours << "hours, " << minutes << "minutes.\n";
}
#include "friendtime.h"

Time::Time()
{
    hours = minutes = 0;
}

Time::Time(int h, int m)
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

Time Time::operator+(const Time & t) const
{
    Time sum;
    sum.minutes = minutes + t.minutes;
    sum.hours = hours + sum.minutes / 60;
    sum.minutes %= 60;
    return sum;
}

Time Time::operator*(double mult) const
{
    Time result;
    long totalminutes = hours * mult * 60 + minutes * mult;
    result.hours = totalminutes / 60;
    result.minutes = totalminutes % 60;
    return result;
}

//友元函数的定义就应该和普通函数一样，因为它并不属于类方法，只是他需要在类声明中声明friendå
std::ostream & operator<<(std::ostream & os, const Time & t) //这个友元函数代替了下边的show函数，使用cout重载<<符号?全程不见cout是什么鬼？因为os就是cout，都是ostream类的对象
{
     os << t.hours << " hours, " << t.minutes << " minutes.\n"; // 这里把<<符号看做*就很好理解了，因为它已经被定义和重载过了，所以你只需要用这个类
     return os; //为什么只用在h文件中声明iostream库就可以？
}
void Time::Show() const 
{
    std::cout << hours << "hours, " << minutes << "minutes.\n";
}
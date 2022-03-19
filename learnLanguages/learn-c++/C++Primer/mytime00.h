#ifndef MYTIME00_H
#define MYTIME00_H

class Time
{
private:
    int hours;
    int minutes;
    
public:
    Time();
    Time(int h, int m = 0);
    ~Time();
    void AddMin(int m);
    void AddHr(int h);
    void Reset(int h = 0, int m = 0);
    Time operator+(const Time & t) const; //返回Sum而不是它的引用，是因为，函数在结束的时候，内部的变量（局部变量）将被删除，但是我们需要在此函数结束之前Sum还是存在，所以这样就会有一个拷贝
    //上面operator+表示重载+运算符，就可以使用+作用于两个Time类对象
    void Show() const;
};


#endif
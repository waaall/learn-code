#ifndef STOCK20_H
#define STOCK20_H

#include <string>

class Stock
{
private:  //private不需要写出来，默认就是private
    std::string company;
    long shares;
    double share_val;
    double total_val;
    /*定义在类声明中的函数将自动成为内联函数，就像下文中的set_tot()
    当然也可以inline声明，但是每个使用这个内联函数的文件都需要定义一遍
    */
    void set_tot() 
    {
        total_val = shares * share_val;
    }

public:
    Stock();  //两个类构造函数，就是使用函数重载（函数名一样，函数参数不一样）
    Stock(const std::string & co, long n = 0, double pr = 0.0);
    ~Stock() //只有一个析构函数
    {
        //cout << "Bye, " << company << "!\n";
    }

    //void acquire(const std::string & co, long n, double pr); //
    void buy(long num, double price);
    void sell(long num, double price);
    void update(double price);
    void show() const; //我们不想让show方法（方法就是类的成员函数）修改对象，我们没法像以前那样在用const，是因为参数并没有被“显式的”传递，所以类就使用了一种“新的”写法实现这个功能，就是const写最后。
    double totalv() const
    {
        return total_val;
    }
    const Stock & topval(const Stock & s) const; //const Stock &是一种“特殊”的类型，是对一个const类对象的引用，上面代码<const std::string & co>也是同样，co是一个string类对象的引用，这个co的const和最后的const分别让传入的类对象和这个类对象不能被改变，传入的类对象是“显式访问”，而“这个”类对象是“隐式”访问
    //值得注意的是：上面这个函数（方法）是可以返回s的，但是“这个”类对象是“隐式”的传递的，如果返回它怎么办呢？->This指针！
};

#endif
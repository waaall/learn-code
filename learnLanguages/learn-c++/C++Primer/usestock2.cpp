#include <iostream>
#include "stock20.h"

const int STKS = 4;
int main()
{
    Stock stocks[STKS] = {
        Stock("NanoSmart", 12, 20.0),
        Stock("Boffo Objects", 200, 2.0),
        Stock("Monolithic Obelisks", 130, 3.25),
        Stock("Fleep Enterprises", 60, 6.5)
        };
    
    std::cout << "Stock holdings:\n";
    int st;
    for (st = 0; st < STKS; st++)
        stocks[st].show();

    const Stock* top = &stocks[0];
    for (st = 1; st < STKS; st++)
        top = &top->topval(stocks[st]); // "->"和"()"运算符优先级高于"&"，所先看右边，top->topval(),这是一个方法返回的值，返回了一个引用，我们再用地址运算符得到地址，赋值给top

    std::cout << "\nMost valuable holding:\n";
    top->show();

    return 0;
}
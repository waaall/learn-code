#include <iostream>
#include "friendtime.h"

int main()
{
    using std::cout;
    using std::endl;

    Time aida(3, 35);
    Time tosca(2, 40);
    Time temp;

    cout << "aida and tosca: ";
    cout << aida << "; " << tosca << endl; //这种显示要比show()函数更直观通用，这就是重载运算符的好处
    temp = aida + tosca;
    cout << "aida + tosca: " << temp << endl;
    temp = aida * 1.17;
    cout << "aida * 1.17: " << temp << endl;
    cout << "10.0 * tosca: " << 10.0 * tosca << endl; 

    return 0;
}
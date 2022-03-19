#include <iostream>
#include "mytime00.h"

int main()
{
    using std::cout;
    using std::endl;

    Time planing;
    Time coding(2, 40);
    Time fixing(5, 55);
    Time total;

    cout << "planing time = ";
    planing.Show();
    cout << endl;

    cout << "coding time = ";
    coding.Show();
    cout << endl;

    cout << "fixing time = ";
    fixing.Show();
    cout << endl;

    total = coding + fixing; //这==“total = coding.operator+(fixing)”
    cout << "coding time = ";
    total.Show();
    cout << endl;

    return 0;
}
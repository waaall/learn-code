#include <cstring>
#include "stringbad.h"

using std::cout;

//initializing static class member
int StringBad::num_strings = 0;

//class methods

//construct StringBad from C string 
StringBad::StringBad(const char *s) //char指针比较特殊，所以我故意把*和char分开写来表示它和其他指针使用的不同
{
    len = std::strlen(s);       // set size
    str = new char[len + 1];    // allot storage
    std::strcpy(str, s);        // initialize pointer
    num_strings++;              // set object count
    cout << num_strings << ": \"" << str //char指针初始化和使用时默认不是地址而是字符串本身
        << "\" object created\n";   // For Your Information
}

StringBad::StringBad()          // default constructor
{
    len = 4;
    str = new char[4];
    std::strcpy(str, "C++");    // default string
    num_strings++;
    cout << num_strings << ": \"" << str
        << "\" default object created\n";   // FYI
}

StringBad::~StringBad()         // necessary destructor
{
    cout << "\"" << str << "\" object deleted, ";   // FYI
    --num_strings;              // required
    cout << num_strings << " left\n";   // FYI
    delete [] str;              // required
}

std::ostream & operator<<(std::ostream & os, const StringBad & st)
{
    os << st.str;
    return os;
}

StringBad::StringBad(const String & s)
{
    num_strings++;
}

StringBad & StringBad::operator=(const StringBad & st)
{
    if (this == &st)            // object assigned to itself
        return *this;           // all done
    delete [] str;              // free old string
    len = st.len;
    str = new char [len + 1];   // get space for new string
    std::strcpy(str, st.str);   // copy the string
    return *this;               // return reference to invoking object
}
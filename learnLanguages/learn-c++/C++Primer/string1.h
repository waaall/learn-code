#ifndef STRING1_H
#define STRING1_H 

#include <iostream>
using std::ostream
using std::istream

class String
{
private:
    char *str;
    int len;
    static int num_strings;
    static const int CINLIM = 80;

public:
    String(const char *s);
    String(const String & s);
    ~String();
    int length() const {return len;} //这是干啥用？跟stock20中topval函数对比
    
    //运算符重载
    String



};


#endif
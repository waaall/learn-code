// https://zh.wikipedia.org/wiki/调度场算法
// https://zh.wikipedia.org/wiki/逆波兰表示法#C++程序实现

// 调度场算法是将中缀表达式转换为后缀表达式，而后者又称逆波兰表示法，是计算机进行数学计算的高效方式
// 然而在实际中，往往会讲中缀表达式转化为表达式树，然后其便利序列就是后缀、前缀、中缀表达式
#include <cstring>
#include <cstdio>
#define op_left_assoc(c) (c == '+' || c == '-' || c == '/' || c == '*' || c == '%')
#define is_operator(c)   (c == '+' || c == '-' || c == '/' || c == '*' || c == '!' || c == '%' || c == '=')
#define is_function(c)   (c >= 'A' && c <= 'Z')
#define is_ident(c)      ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z'))

// 操作符
// 优先级  符号  运算顺序
// 1        !       从右至左
// 2        * / %   从左至右
// 3        + -     从左至右
// 4        =       从右至左
int op_preced(const char c)
{
    switch (c)
    {
    case '!':
        return 4;
    case '*':  case '/': case '%':
        return 3;
    case '+': case '-':
        return 2;
    case '=':
        return 1;
    }
    //若输入不是运算符
    return 0;
}

unsigned int op_arg_count(const char c)
{
    switch (c)
    {
        //运算符
    case '*': case '/': case '%': case '+': case '-': case '=':
        return 2;
        //阶乘
    case '!':
        return 1;
        //不是运算符
    default:
        return c - 'A';
    }
    return 0;
}

bool shunting_yard(const char* input, char* output)
{
    const char* strpos = input, * strend = input + strlen(input);
    char c, stack[32], sc, * outpos = output;
    unsigned int sl = 0;
    while (strpos < strend)
    {
        c = *strpos;
        if (c != ' ')
        {    
            // 扫描到左括号直接入栈
            if (c == '(')
            {
                stack[sl] = c;
                ++sl;
            }
            // 如果输入为数字，则直接入输出队列
            else if (is_ident(c))
            {
                *outpos = c;
                ++outpos;
            }
            // 如果输入为函数记号，则压入堆栈
            else if (is_function(c))
            {
                stack[sl] = c;
                ++sl;
            }
            // 如果输入为函数分割符（如:逗号）
            else if (c == ',')
            {
                bool pe = false;
                while (sl > 0)
                {
                    sc = stack[sl - 1];
                    //扫描到左括号
                    //跳出输出循环，此时左括号作为函数边界判定，所以不出栈
                    if (sc == '(')
                    {
                        pe = true;
                        break;
                    }
                    else {
                        // 栈顶元素不是左括号
                        // 将栈顶元素依次出栈并放入输出队列
                        *outpos = sc;
                        ++outpos;
                        sl--;
                    }
                }
                // 如果没有遇到左括号，则有可能是符号放错或者不匹配
                if (!pe)
                {
                    printf("Error: separator or parentheses mismatched\n");
                    return false;
                }
            }
            // 如果输入符号为运算符，然后:
            else if (is_operator(c))
            {
                while (sl > 0)
                {
                    sc = stack[sl - 1];
                    // sc为其栈顶元素
                    // 如果c是左结合性的且它的优先级小于等于栈顶运算符sc的优先级
                    // 或者c是右结合性且它的优先级小于栈顶运算符sc的优先级
                    // 将栈顶元素sc出栈，否则sc进栈
                    if (is_operator(sc) && ((op_left_assoc(c) && (op_preced(c) <= op_preced(sc))) || 
                        (!op_left_assoc(c) && (op_preced(c) < op_preced(sc))))) 
                    {
                        *outpos = sc;
                        ++outpos;
                        sl--;
                    }
                    else
                    {
                        break;
                    }
                }
                //c的优先级大于或大于等于结合性的要求，则将其入栈
                stack[sl] = c;
                ++sl;
            }

            // 扫描到右括号
            else if (c == ')')
            {
                bool pe = false;
                // 从栈顶向下扫描左括号，将扫描到左括号之前的栈顶运算符出栈并放入输出队列
                while (sl > 0)
                {
                    sc = stack[sl - 1];
                    if (sc == '(')
                    {
                        pe = true;
                        break;
                    }
                    else
                    {
                        *outpos = sc;
                        ++outpos;
                        sl--;
                    }
                }
                // 如果没有扫描到左括号，则有可能是符号放错或者不匹配
                if (!pe)
                {
                    printf("Error: parentheses mismatched\n");
                    return false;
                }
                // 左括号出栈且不放入输出队列
                sl--;
                // 扫描完左括号后 
                // 如果栈顶元素是函数运算符
                // 则将其出栈并放入输出队列
                if (sl > 0)
                {
                    sc = stack[sl - 1];
                    if (is_function(sc))
                    {
                        *outpos = sc;
                        ++outpos;
                        sl--;
                    }
                }
            }
            //未知运算符c
            else printf("Unknown token %c\n", c);
        }
        ++strpos;
    }
    // 当所有元素已经读完
    // 栈中还有剩余运算符
    while (sl > 0)
    {
        sc = stack[sl - 1];
        //如果剩余括号，则符号放错或者不匹配
        if (sc == '(' || sc == ')') 
        {
            printf("Error: parentheses mismatched\n");
            return false;
        }
        //出栈并放入输出队列
        *outpos = sc;
        ++outpos;
        --sl;
    }
    *outpos = 0;//指针置零
    return true;
}

bool execution_order(const char* input) 
{
    printf("order: (arguments in reverse order)\n");
    const char* strpos = input, * strend = input + strlen(input);
    char c, res[4];
    unsigned int sl = 0, sc, stack[32], rn = 0;
    // While there are input tokens left
    while (strpos < strend) 
    {
        // Read the next token from input.
        c = *strpos;
        // If the token is a value or identifier
        if (is_ident(c)) 
        {
            // Push it onto the stack.
            stack[sl] = c;
            ++sl;
        }
        // Otherwise, the token is an operator  (operator here includes both operators, and functions).
        else if (is_operator(c) || is_function(c))
        {
            sprintf(res, "_%02d", rn);
            printf("%s = ", res);
            ++rn;
            // It is known a priori that the operator takes n arguments.
            unsigned int nargs = op_arg_count(c);
            // If there are fewer than n values on the stack
            if (sl < nargs ) return false; // (Error) The user has not input sufficient values in the expression.
            // Else, Pop the top n values from the stack.
            // Evaluate the operator, with the values as arguments.
            if (is_function(c)) 
            {
                printf("%c(", c);
                while (nargs > 0) 
                {
                    sc = stack[sl - 1];
                    sl--;
                    if (nargs > 1) printf("%s, ", &sc);
                    else printf("%s)\n", &sc);
                    --nargs;
                }
            }
            else 
            {
                if (nargs == 1) 
                {
                    sc = stack[sl - 1];
                    sl--;
                    printf("%c %s;\n", c, &sc);
                }
                else
                {
                    sc = stack[sl - 1];
                    sl--;
                    printf("%s %c ", &sc, c);
                    sc = stack[sl - 1];
                    sl--;
                    printf("%s;\n", &sc);
                }
            }
            // Push the returned results, if any, back onto the stack.
            stack[sl] = *(unsigned int*)res;
            ++sl;
        }
        ++strpos;
    }
    // If there is only one value in the stack
    // That value is the result of the calculation.
    if (sl == 1) 
    {
        sc = stack[sl - 1];
        sl--;
        printf("%s is a result\n", &sc);
        return true;
    }
    // If there are more values in the stack
    // (Error) The user input has too many values.
    return false;
}

int main()
{
    // functions: A() B(a) C(a, b), D(a, b, c) ...
    // identifiers: 0 1 2 3 ... and a b c d e ...
    // operators: = - + / * % !
    const char* input = "a = D(f - b * c + d, !e, g)";
    char output[128];
    printf("input: %s\n", input);
    if (shunting_yard(input, output)) 
    {
        printf("output: %s\n", output);
        if (!execution_order(output))
            printf("\nInvalid input\n");
    }
    return 0;
}
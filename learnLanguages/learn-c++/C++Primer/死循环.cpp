#include <iostream>
#include <string>

using namespace std;

int main()
{
    int ch;

    for (int i = 0; i < 5; ++i)
    {
        cout << "enter the customer structure/fullname:";

        cin >> ch;
        while (cin.get() != '\n') //如果输入字母，就会死循环
        {
            printf("In a loop: \n");
            cout << "cin.get: " << cin.get() << endl;             
        }

        cout <<"this is ch: " << ch << endl;
    }

	return 0;
}

#include <iostream>
#include <string>
#include <new>

using namespace std;
const int BUF = 512;

class JustTesting
{
private:
    string words;
    int number;

public:
    JustTesting(const string & s = "Just Testing", int n = 0)
    {
        words = s; 
        number = n;
        cout << words << "constructed\n";
    }
    ~JustTesting(){ cout << words << "destroysed\n";}

    void Show() const { cout << words << ", " << number << endl;}
    
};

int main(int argc, char const *argv[])
{
    char * buffer = new char[BUF];
    JustTesting *pc1, *pc2;
    pc1 = new (buffer) JustTesting;
    pc2 = new JustTesting("Heap1", 20);

    cout << "Memory block addresses:\n" << "buffer: "
        << (void *) buffer << "  heap: " << pc2 << endl;

    cout << "Memory contents: \n";
    cout << pc1 << ": ";
    pc1->Show();
    cout << pc2 << ": ";
    pc2->Show();

    

    return 0;
}
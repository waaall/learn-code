// 用指针传递结构参数

#include <iostream>
#include <string>
#include <cmath>

struct polar
{
    double distance;
    double angle;
};
struct rect
{
    double x;
    double y;
};

void show_polar(const polar* dapos);
void rect_to_polar(rect* xypos, polar* dapos);


int main()
{
    using namespace std;
    rect xypos;
    polar answer;

    cout << "Type your x position: ";
    cin >> xypos.x;
    cout << "Type your y position: ";
    cin >> xypos.y;

    rect_to_polar(&xypos, &answer);

    cout << "The Adress of the polar struct 'answer' is " << &answer << endl;
    show_polar(&answer);

    return 0;
}


void show_polar (const polar* dapos)
{
    using namespace std;
    const double Rad_to_deg = 57.29577951;

    cout << "distance = " << dapos->distance;
    cout << endl << "angle = " << dapos->angle * Rad_to_deg;
    cout << " degrees\n";
}


void rect_to_polar(rect* xypos, polar* answer)
{
    using namespace std;

    answer->distance = sqrt(xypos->x * xypos->x + xypos->y * xypos->y);
    answer->angle = atan2(xypos->y , xypos->x);

}
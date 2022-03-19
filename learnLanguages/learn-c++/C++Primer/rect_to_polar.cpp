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

void show_polar(polar dapos);
polar rect_to_polar(rect xypos);


int main()
{
    using namespace std;
    int x, y;
    rect xypos;

    cout << "Type your x position: ";
    cin >> x;
    cout << "Type your y position: ";
    cin >> y;
    xypos.x = x;
    xypos.y = y;

    show_polar(rect_to_polar(xypos));

    return 0;
}


void show_polar (polar dapos)  //此时 参数用“const polar * dapos” 就是用指针传递参数而不是结构本身，这样可以节省拷贝数据的时间，const是避免这个函数修改该结构
{
    using namespace std;
    const double Rad_to_deg = 57.29577951;

    cout << "distance = " << dapos.distance;
    cout << endl << "angle = " << dapos.angle * Rad_to_deg;
    cout << " degrees\n";
}


polar rect_to_polar(rect xypos)
{
    using namespace std;
    polar answer;

    answer.distance = sqrt(xypos.x * xypos.x + xypos.y * xypos.y);
    answer.angle = atan2(xypos.y , xypos.x);

    return answer;
}
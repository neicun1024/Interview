#include <bits/stdc++.h>
using namespace std;

const double PI = 3.14159265358979323846;
const double eps = 1e-6;

class Line
{
public:
    double a;
    double b;
    double c;
    // ax+by+c = 0;
};

double get_Line_Angle(Line line)
{
    double a = line.a, b = line.b, c = line.c;

    if (a == 0)
    {
        return 0;
    }
    else if (b == 0)
    {
        return PI / 2;
    }
    else
    {
        return atan(-a / b);
    }
}

double getRelativeAngle(double x1, double y1, double x2, double y2)
{
    double refx = x2 - x1, refy = y2 - y1;

    // 两点重合直接认为角度为0
    if (abs(refx) < eps && abs(refy) < eps)
    {
        return 0;
    }

    if (abs(refx) < eps)
    {
        if (refy > 0)
        {
            return PI / 2;
        }
        else
        {
            return 3 * PI / 2;
        }
    }
    if (refx < 0)
    {
        return (PI + atan(refy / refx));
    }
    else
    {
        if (abs(refy) < eps)
        {
            return atan(refy / refx);
        }
        else
        {
            return ((-refy + abs(refy)) / abs(refy) * PI + atan(refy / refx));
        }
    }
}

double getDistance(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int main()
{
    double x = 38.652796101132523;
    double y = 26.903720088441215;

    double c1x = 42.614257553539041;
    double c1y = 49.088948887815121;
    double c1r = 0.86602540378443793;

    double c2x = 37.239530206026046;
    double c2y = 49.099255125581969;
    double c2r = 6.2407626325837207;

    // double angle1 = getRelativeAngle(x, y, p1x, p1y);
    // double angle2 = getRelativeAngle(x, y, p2x, p2y);

    cout << "distance = " << getDistance(c1x, c1y, c2x, c2y) << endl;
    // cout << "distance2 = " << getDistance(x, y, p2x, p2y) << endl;
    // cout << "angle1 = " << angle1 << endl;
    // cout << "angle1 = " << angle2 << endl;

    // cout<<getDistance(0.86602540378443793, y, 40.437025055696282, 28.687949043004970) << endl;

    cout << "r1 - r2 = " << c1r - c2r << endl;
    cout << "r2 - r1 = " << c2r - c1r << endl;
    cout << "r1 + r2 = " << c1r + c2r << endl;

    cout << 2.0955850343700977 + 1.0471975511965976 << endl;
    cout << 2.0955850343700977 + 1.0471975511965976 - PI << endl;
    return 0;
}
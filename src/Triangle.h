#include "Point.h"
class Triangle{
private:
    Point a;
    Point b;
    Point c;
public:
    Triangle(Point, Point, Point);
    Point getA();
    Point getB();
    Point getC();
    void setA(Point a);
    void setB(Point b);
    void setC(Point c);
    double meanZ();
};



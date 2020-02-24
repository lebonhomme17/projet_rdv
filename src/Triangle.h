#include "Point.h"
class Triangle{
private:
    Point x;
    Point y;
    Point z;
public:
    Triangle(Point x, Point y, Point z);
    Point getX();
    Point getY();
    Point getZ();
    void setX(Point x);
    void setY(Point y);
    void setZ(Point Z);
};



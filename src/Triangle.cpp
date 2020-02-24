//
// Created by asus on 24/02/2020.
//

#include "Triangle.h"


Triangle::Triangle(Point px, Point py, Point pz) : x(px), y(py), z(pz){}
Point Triangle::getX() {
    return x;
}
Point Triangle::getY() {
    return y;
}
Point Triangle::getZ() {
    return z;
}
void Triangle::setX(Point x) {
    this->x = x;
}
void Triangle::setY(Point y) {
    this->y = y;
}
void Triangle::setZ(Point z) {
    this->z = z;
}
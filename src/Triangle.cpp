//
// Created by asus on 24/02/2020.
//

#include "Triangle.h"


Triangle::Triangle(Point pa, Point pb, Point pc) : a(pa), b(pb), c(pc){}
Point Triangle::getA() {
    return a;
}
Point Triangle::getB() {
    return b;
}
Point Triangle::getC() {
    return c;
}
void Triangle::setA(Point a) {
    this->a = a;
}
void Triangle::setB(Point b) {
    this->b = b;
}
void Triangle::setC(Point c) {
    this->c = c;
}
double Triangle::meanZ() {
    return (getA().getZ()+getB().getZ()+getC().getZ())/3;
}


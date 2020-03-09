#ifndef __GEOM_H__
#define __GEOM_H__

#include <iostream>

template <size_t DIM, typename T> struct vec {
    vec() { for (size_t i=DIM; i--; data_[i] = T()); }
    T& operator[](const size_t i) { assert(i<DIM); return data_[i]; }
    const T& operator[](const size_t i) const { assert(i<DIM); return data_[i]; }
private:
    T data_[DIM];
};

typedef vec<3, float> Vec3f;
typedef vec<2, int> Vec2i;
typedef vec<2, float> Vec2f;

#endif //__GEOM_H__
#ifndef GEOMETRY_H
#define GEOMETRY_H

#define T_VAR   float

class point3 {

public:
    point3(T_VAR _x = 0.f, T_VAR _y = 0.f, T_VAR _z = 0.f) : x(_x), y(_y), z(_z) {}
    T_VAR x, y, z;

};

class triangle {

public:
    triangle() {}
    triangle(int _s1, int _s2, int _s3) : s1(_s1), s2(_s2), s3(_s3) {}

    int s1, s2, s3;
};

#endif // GEOMETRY_H

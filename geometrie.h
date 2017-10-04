#ifndef GEOMETRIE
#define GEOMETRIE

#include <ostream>

#define VAR_TYPE    float


class Point {
public:
    Point(VAR_TYPE _x = 0, VAR_TYPE _y = 0, VAR_TYPE _z = 0) : x(_x), y(_y), z(_z) {}

    friend std::ostream & operator << (std::ostream & out, const Point & p) { return out << p.x << " " << p.y << " " << p.z; }

    VAR_TYPE x, y, z;
};

class Vector3 {
public:
    Vector3(VAR_TYPE _x = 0, VAR_TYPE _y = 0, VAR_TYPE _z = 0) : x(_x), y(_y), z(_z) {}
    Vector3(Point p1, Point p2) : x(p2.x - p1.x), y(p2.y - p1.y), z(p2.z - p1.z) {}

    friend std::ostream & operator << (std::ostream & out, const Vector3 & v) { return out << v.x << " " << v.y << " " << v.z; }

    static Vector3 cross(Vector3 u, Vector3 v) { return Vector3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x); }

    VAR_TYPE x, y, z;
};

class Sommet {
public:
    Sommet() {}
    Sommet(Point p, int f = -1) : coord(p), face(f) {}

    friend std::ostream & operator << (std::ostream & out, const Sommet & s) { return out << s.coord << " | face voisine : " << s.face; }

    Point coord;
    int face;
};

class Triangle {
public:
    Triangle() {}
    Triangle(int s1, int s2, int s3, int v1 = -1, int v2 = -1, int v3 = -1) : s{s1, s2, s3}, v{v1, v2, v3} {}

    friend std::ostream & operator << (std::ostream & out, const Triangle & t) { return out << "s : " << t.s[0] << " " << t.s[1] << " " << t.s[2] << " | v : " << t.v[0] << " " << t.v[1] << " " << t.v[2]; }

    int s[3];
    int v[3];
};

#endif // GEOMETRIE


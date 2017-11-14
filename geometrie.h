#ifndef GEOMETRIE
#define GEOMETRIE

#include<iostream>
#include<math.h>
#include<vector>

#define VAR_TYPE    float


class Point {
public:
    Point(VAR_TYPE _x = 0, VAR_TYPE _y = 0, VAR_TYPE _z = 0) : x(_x), y(_y), z(_z) {}

    static float dist(Point a, Point b) { return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)); }
    friend std::ostream & operator << (std::ostream & out, const Point & p) { return out << p.x << " " << p.y << " " << p.z; }

    VAR_TYPE x, y, z;
};

class Vector3 {
public:
    Vector3(VAR_TYPE _x = 0, VAR_TYPE _y = 0, VAR_TYPE _z = 0) : x(_x), y(_y), z(_z) {}
    Vector3(Point p1, Point p2) : x(p2.x - p1.x), y(p2.y - p1.y), z(p2.z - p1.z) {}
    Vector3(Point p):x(p.x), y(p.y), z(p.z){}

    static float dot(Vector3 u, Vector3 v) { return (u.x*v.x + u.y*v.y + u.z*v.z); }
    static Vector3 cross(Vector3 u, Vector3 v) { return Vector3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x); }
    static Vector3 abss(Vector3 other){ Vector3 r; r.x =abs(other.x); r.y=abs(other.y); r.z=abs(other.z); return r; }
    friend std::ostream & operator << (std::ostream & out, const Vector3 & v) { return out << v.x << " " << v.y << " " << v.z; }

    VAR_TYPE x, y, z;
};

inline Vector3 operator -(Vector3 v, Vector3 u){Vector3 res; res.x = v.x-u.x; res.y = v.y-u.y; res.z = v.z-u.z; return res;}
inline Vector3 operator +(Vector3 v, Vector3 u){Vector3 res; res.x = v.x+u.x; res.y = v.y+u.y; res.z = v.z+u.z; return res;}
inline Vector3 operator *(Vector3 v, Vector3 u){Vector3 res; res.x = v.x*u.x; res.y = v.y*u.y; res.z = v.z*u.z; return res;}
inline Vector3 operator *(Vector3 u, float other){Vector3 r; r.x=other*u.x; r.y=other*u.y; r.z=other*u.z;  return r;}
inline Vector3 operator *(float other, Vector3 u){Vector3 r; r.x=other*u.x; r.y=other*u.y; r.z=other*u.z;  return r;}


class Sommet {
public:
    Sommet() {}
    Sommet(Point p, int f = -1) : coord(p), face(f) {}

    int getFace() { return face; }
    Point getPoint() { return coord; }
    void setFace(int newFace) { face = newFace; }

    bool operator ==(Sommet & comp) { if(this == &comp) return true; return false; }
    friend std::ostream & operator << (std::ostream & out, const Sommet & s) { return out << s.coord << " | face voisine : " << s.face; }

    Point coord;
    int face;
};

class Triangle {
public:
    Triangle() {}
    Triangle(int s1, int s2, int s3, int v1 = -1, int v2 = -1, int v3 = -1) : s{s1, s2, s3}, v{v1, v2, v3} {}

    int (&getVoisins())[3]{return v;};
    int (&getSommets())[3]{return s;};

    friend std::ostream & operator << (std::ostream & out, const Triangle & t) { return out << "s : " << t.s[0] << " " << t.s[1] << " " << t.s[2] << " | v : " << t.v[0] << " " << t.v[1] << " " << t.v[2]; }

private:
    int s[3];
    int v[3];
};


class Delaunay{
public:
    static bool isOutCircle(Point p1, Point p2, Point p3, Point np);
    static double deter(int n, double mat[10][10], double & d);
};

class CercleC{
public:
    CercleC():center(Point()), radius(0){}

    Point center;
    float radius;
};

class VoronoiCell{
public:
    VoronoiCell(){}
    ~VoronoiCell(){}
    std::vector<int> & getPoints() { return points; }

private:
    std::vector<int> points;
};

#endif // GEOMETRIE


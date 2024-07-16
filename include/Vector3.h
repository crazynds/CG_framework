#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__

#include <math.h>

template <class T>
class Vector3
{
public:
    T x, y, z;

    Vector3()
    {
        x = y = z = 0;
    }

    Vector3(T _x, T _y, T _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void set(T _x, T _y, T _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    float size()
    {
        return (float)sqrt(x * x + y * y + z * z);
    }

    float normalize()
    {
        float norm = (float)sqrt(x * x + y * y + z * z);

        if (norm == 0.0)
        {
            x = 1;
            y = 0;
            z = 0;
            return norm;
        }
        x /= norm;
        y /= norm;
        return norm;
    }

    Vector3<T> operator-(const Vector3<T> &v)
    {
        Vector3<T> aux(x - v.x, y - v.y, z - v.z);
        return (aux);
    }

    Vector3<T> operator+(const Vector3<T> &v)
    {
        Vector3<T> aux(x + v.x, y + v.y, z + v.z);
        return (aux);
    }

    Vector3<T> operator*(const T &v)
    {
        Vector3<T> aux(x * v, y * v, z * v);
        return (aux);
    }

    Vector3<T> &operator+=(const Vector3<T> &v)
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        return *this;
    }

    Vector3<T> &operator-=(const Vector3<T> &v)
    {
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
        return *this;
    }

    Vector3<T> &operator*=(const T &v)
    {
        this->x *= v;
        this->y *= v;
        this->z *= v;
        return *this;
    }

    // Produto escalar (dot product)
    double dot(const Vector3<T> &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    // Produto vetorial (cross product)
    double cross(const Vector3<T> &v) const
    {
        return x * v.y - y * v.x - z * v.z;
    }
};

class Vector3i : public Vector3<int>
{
public:
    Vector3i() : Vector3() {}
    Vector3i(int x, int y, int z) : Vector3(x, y, z) {}
    Vector3i(Vector3<int> vec) : Vector3(vec.x, vec.y, vec.y) {}
};
class Vector3f : public Vector3<float>
{
public:
    Vector3f() : Vector3() {}
    Vector3f(float x, float y, float z) : Vector3(x, y, z) {}
};
class Vector3d : public Vector3<double>
{
public:
    Vector3d() : Vector3() {}
    Vector3d(double x, double y, double z) : Vector3(x, y, z) {}
    Vector3d(const Vector3i &vec) : Vector3(vec.x, vec.y, vec.z) {}
    Vector3d(const Vector3<double> &vec) : Vector3(vec.x, vec.y, vec.z) {}
};

#endif

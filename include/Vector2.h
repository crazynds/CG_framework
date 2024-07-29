#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <math.h>

template <class T>
class Vector2
{
public:
    T x, y;

    Vector2()
    {
        x = y = 0;
    }

    Vector2(T _x, T _y)
    {
        x = _x;
        y = _y;
    }

    void set(T _x, T _y)
    {
        x = _x;
        y = _y;
    }

    float size() const
    {
        return (float)sqrt(x * x + y * y);
    }

    float normalize()
    {
        float norm = (float)sqrt(x * x + y * y);

        if (norm == 0.0)
        {
            x = 1;
            y = 1;
            return norm;
        }
        x /= norm;
        y /= norm;
        return norm;
    }

    Vector2<T> operator-(const Vector2<T> &v)
    {
        Vector2<T> aux(x - v.x, y - v.y);
        return (aux);
    }

    Vector2<T> operator+(const Vector2<T> &v)
    {
        Vector2<T> aux(x + v.x, y + v.y);
        return (aux);
    }

    Vector2<T> operator*(const T &v)
    {
        Vector2<T> aux(x * v, y * v);
        return (aux);
    }

    Vector2<T> &operator+=(const Vector2<T> &v)
    {
        this->x += v.x;
        this->y += v.y;
        return *this;
    }

    Vector2<T> &operator-=(const Vector2<T> &v)
    {
        this->x -= v.x;
        this->y -= v.y;
        return *this;
    }

    Vector2<T> &operator*=(const T &v)
    {
        this->x *= v;
        this->y *= v;
        return *this;
    }

    // Produto escalar (dot product)
    double dot(const Vector2<T> &v) const
    {
        return x * v.x + y * v.y;
    }

    // Produto vetorial (cross product)
    double cross(const Vector2<T> &v) const
    {
        return x * v.y - y * v.x;
    }

    float angleWith(const Vector2<T> &other) const
    {
        float dotProduct = dot(other);
        float magnitudes = size() * other.size();
        float cosTheta = dotProduct / magnitudes;

        return std::acos(cosTheta);
    }

    // Adicionem os demais overloads de operadores aqui.
};

class Vector2i : public Vector2<int>
{
public:
    Vector2i() : Vector2() {}
    Vector2i(int x, int y) : Vector2(x, y) {}
    Vector2i(Vector2<int> vec) : Vector2(vec.x, vec.y) {}
};
class Vector2f : public Vector2<float>
{
public:
    Vector2f() : Vector2() {}
    Vector2f(float x, float y) : Vector2(x, y) {}
};
class Vector2d : public Vector2<double>
{
public:
    Vector2d() : Vector2() {}
    Vector2d(double x, double y) : Vector2(x, y) {}
    Vector2d(const Vector2i &vec) : Vector2(vec.x, vec.y) {}
    Vector2d(const Vector2<double> &vec) : Vector2(vec.x, vec.y) {}
};

#endif

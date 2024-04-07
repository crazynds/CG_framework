#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

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

    void normalize()
    {
        float norm = (float)sqrt(x * x + y * y);

        if (norm == 0.0)
        {
            x = 1;
            y = 1;
            return;
        }
        x /= norm;
        y /= norm;
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

    Vector2<T> *operator*=(const T &v)
    {
        this->x *= v;
        this->y *= v;
        return *this;
    }

    // Adicionem os demais overloads de operadores aqui.
};

class Vector2i : public Vector2<int>
{
public:
    Vector2i() : Vector2() {}
    Vector2i(int x, int y) : Vector2(x, y) {}
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
};

#endif

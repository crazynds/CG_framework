#ifndef __MATRIX_2_H__
#define __MATRIX_2_H__

#include <Vector2.h>
#include <vector>
#include <math.h>
#include <string.h>

/**
 * Classes auxiliares para trabalhar com matrizes.
 */

template <class T>
class Matrix3
{
public:
    T m[3][3];

    Matrix3()
    {
        for (int x = 0; x < 3; x++)
        {
            for (int y = 0; y < 3; y++)
            {
                m[x][y] = 0;
            }
        }
    }

    Matrix3(const Matrix3<T> &mat)
    {
        memcpy(m, mat.m, sizeof(T) * 9);
    }

    Matrix3(Vector2<T> vec) : Matrix3()
    {
        m[0][0] = vec.x;
        m[1][1] = vec.y;
        m[2][2] = 1;
    };

    static Matrix3 identity()
    {
        Matrix3 mat = Matrix3();
        mat.m[0][0] = 1;
        mat.m[1][1] = 1;
        mat.m[2][2] = 1;
        return mat;
    }
    static Matrix3 rotation(T angle)
    {
        Matrix3 mat = Matrix3();
        mat.m[0][0] = cos(angle);
        mat.m[0][1] = -sin(angle);
        mat.m[1][0] = sin(angle);
        mat.m[1][1] = cos(angle);
        mat.m[2][2] = 1;
        return mat;
    }
    static Matrix3 translation(T x, T y)
    {
        Matrix3 mat = Matrix3::identity();
        mat.m[0][2] = x;
        mat.m[1][2] = y;
        return mat;
    }
    static Matrix3 scale(T x, T y)
    {
        Matrix3 mat = Matrix3::identity();
        mat.m[0][0] = x;
        mat.m[1][1] = y;
        return mat;
    }
    Matrix3<T> &operator+=(const Matrix3<T> &mat)
    {
        m[0][0] += mat.m[0][0];
        m[0][1] += mat.m[0][1];
        m[0][2] += mat.m[0][2];
        m[1][0] += mat.m[1][0];
        m[1][1] += mat.m[1][1];
        m[1][2] += mat.m[1][2];
        m[2][0] += mat.m[2][0];
        m[2][1] += mat.m[2][1];
        m[2][2] += mat.m[1][2];
        return *this;
    }
    Matrix3<T> &operator-=(const Matrix3<T> &mat)
    {
        m[0][0] -= mat.m[0][0];
        m[0][1] -= mat.m[0][1];
        m[0][2] -= mat.m[0][2];
        m[1][0] -= mat.m[1][0];
        m[1][1] -= mat.m[1][1];
        m[1][2] -= mat.m[1][2];
        m[2][0] -= mat.m[2][0];
        m[2][1] -= mat.m[2][1];
        m[2][2] -= mat.m[1][2];
        return *this;
    }
    Matrix3<T> operator*(const T val)
    {
        Matrix3<T> aux(this);
        aux *= val;
        return aux;
    }
    Matrix3<T> operator/(const T val)
    {
        Matrix3<T> aux(this);
        aux /= val;
        return aux;
    }

    Matrix3<T> &operator=(const Matrix3<T> &mat)
    {
        memcpy(m, mat.m, sizeof(T) * 9);
        return *this;
    }
    Matrix3<T> operator*(const Matrix3<T> &mat)
    {
        Matrix3<T> val = Matrix3<T>(*this);
        val *= mat;
        return val;
    }
    Vector2<T> operator*(const Vector2<T> &vet)
    {
        T val[3] = {vet.x, vet.y, 1};
        T val2[3] = {0, 0, 0};
        for (int x = 0; x < 3; x++)
        {
            for (int y = 0; y < 3; y++)
            {
                val2[x] += m[x][y] * val[y];
            }
        }
        return Vector2<T>(val2[0] / val2[2], val2[1] / val2[2]);
    }
    Matrix3<T> operator*=(const Matrix3<T> &mat)
    {
        Matrix3<T> val = Matrix3<T>();
        for (int x = 0; x < 3; x++)
        {
            for (int y = 0; y < 3; y++)
            {
                for (int z = 0; z < 3; z++)
                {
                    val.m[x][y] += m[x][z] * mat.m[z][y];
                }
            }
        }
        *this = val;
        return *this;
    }

    void print()
    {
        printf("MATRIX:\n");
        for (int x = 0; x < 3; x++)
        {
            for (int y = 0; y < 3; y++)
            {
                printf("%7.1f|", m[x][y]);
            }
            printf("\n");
        }
    }
};

template <class T>
class Matrix3Plan
{
private:
    std::vector<Matrix3<T>> transformation;

public:
    Matrix3Plan()
    {
        clear();
    }

    void clear()
    {
        transformation.clear();
        transformation.push_back(Matrix3<T>::identity());
    }

    void push()
    {
        transformation.push_back(Matrix3<T>::identity());
    }

    void addMultMatrix(Matrix3<T> mat)
    {
        transformation.back() *= mat;
    }

    void addScaleMatrix(T x, T y)
    {
        addMultMatrix(Matrix3<T>::scale(x, y));
    }

    void addRotateMatrix(T angle)
    {
        addMultMatrix(Matrix3<T>::rotation(angle));
    }

    void addTranslateMatrix(T x, T y)
    {
        addMultMatrix(Matrix3<T>::translation(x, y));
    }
    void addTranslateMatrix(Vector2<T> vec)
    {
        addMultMatrix(Matrix3<T>::translation(vec.x, vec.y));
    }

    Matrix3<T> getTransformationMatrix()
    {
        return transformation.back();
    }

    Matrix3<T> compute(Matrix3<T> mat)
    {
        for (Matrix3<T> x : transformation)
        {
            mat = x * mat;
        }
        return mat;
    }

    Vector2<T> compute(Vector2<T> vec)
    {
        for (Matrix3<T> x : transformation)
        {
            vec = x * vec;
        }
        return vec;
    }
};

#endif
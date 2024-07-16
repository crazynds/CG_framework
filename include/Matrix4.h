#ifndef __MATRIX_4_H__
#define __MATRIX_4_H__

#include <Vector2.h>
#include <Vector3.h>
#include <vector>
#include <math.h>
#include <string.h>

/**
 * Classes auxiliares para trabalhar com matrizes.
 */

template <class T>
class Matrix4
{
public:
    T m[4][4];

    Matrix4()
    {
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                m[x][y] = 0;
            }
        }
    }

    Matrix4(const Matrix4<T> &mat)
    {
        memcpy(m, mat.m, sizeof(T) * 16);
    }

    Matrix4(Vector3<T> vec) : Matrix4()
    {
        m[0][0] = vec.x;
        m[1][1] = vec.y;
        m[2][2] = vec.z;
        m[3][3] = 1;
    };

    static Matrix4 identity()
    {
        Matrix4 mat = Matrix4();
        mat.m[0][0] = 1;
        mat.m[1][1] = 1;
        mat.m[2][2] = 1;
        mat.m[3][3] = 1;
        return mat;
    }
    static Matrix4 rotationZ(T angle)
    {
        Matrix4 mat = Matrix4::identity();
        mat.m[0][0] = cos(angle);
        mat.m[0][1] = -sin(angle);
        mat.m[1][0] = sin(angle);
        mat.m[1][1] = cos(angle);
        return mat;
    }

    static Matrix4 projection(T d)
    {
        T r = 1 / d;
        Matrix4 mat = Matrix4::identity();
        mat.m[2][2] = 0;
        mat.m[3][2] = r;
        mat.m[3][3] = 0;
        return mat;
    }

    static Matrix4 rotationY(T angle)
    {
        Matrix4 mat = Matrix4::identity();
        mat.m[0][0] = cos(angle);
        mat.m[0][2] = -sin(angle);
        mat.m[2][0] = sin(angle);
        mat.m[2][2] = cos(angle);
        return mat;
    }

    static Matrix4 rotationX(T angle)
    {
        Matrix4 mat = Matrix4::identity();
        mat.m[1][1] = cos(angle);
        mat.m[1][2] = -sin(angle);
        mat.m[2][1] = sin(angle);
        mat.m[2][2] = cos(angle);
        return mat;
    }
    static Matrix4 translation(T x, T y, T z)
    {
        Matrix4 mat = Matrix4::identity();
        mat.m[0][3] = x;
        mat.m[1][3] = y;
        mat.m[2][3] = z;
        return mat;
    }
    static Matrix4 scale(T x, T y, T z)
    {
        Matrix4 mat = Matrix4::identity();
        mat.m[0][0] = x;
        mat.m[1][1] = y;
        mat.m[2][2] = z;
        return mat;
    }
    Matrix4<T> &operator+=(const Matrix4<T> &mat)
    {
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                m[x][y] += mat.m[x][y];
            }
        }
        return *this;
    }
    Matrix4<T> &operator-=(const Matrix4<T> &mat)
    {
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                m[x][y] -= mat.m[x][y];
            }
        }
        return *this;
    }
    Matrix4<T> operator*(const T val)
    {
        Matrix4<T> aux(this);
        aux *= val;
        return aux;
    }
    Matrix4<T> operator/(const T val)
    {
        Matrix4<T> aux(this);
        aux /= val;
        return aux;
    }

    Matrix4<T> &operator=(const Matrix4<T> &mat)
    {
        memcpy(m, mat.m, sizeof(T) * 16);
        return *this;
    }
    Matrix4<T> operator*(const Matrix4<T> &mat)
    {
        Matrix4<T> val = Matrix4<T>(*this);
        val *= mat;
        return val;
    }
    Vector3<T> operator*(const Vector3<T> &vet)
    {
        T val[4] = {vet.x, vet.y, vet.z, 1};
        T val2[4] = {0, 0, 0, 0};
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                val2[x] += m[x][y] * val[y];
            }
        }
        return Vector3<T>(val2[0] / val2[3], val2[1] / val2[3], val2[2] / val2[3]);
    }
    Matrix4<T> operator*=(const Matrix4<T> &mat)
    {
        Matrix4<T> val = Matrix4<T>();
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                for (int z = 0; z < 4; z++)
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
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                printf("%7.1f|", m[x][y]);
            }
            printf("\n");
        }
    }
};

template <class T>
class Matrix4Plan
{
private:
    std::vector<Matrix4<T>> transformation;

public:
    Matrix4Plan()
    {
        clear();
    }

    void clear()
    {
        transformation.clear();
        transformation.push_back(Matrix4<T>::identity());
    }

    void push()
    {
        transformation.push_back(Matrix4<T>::identity());
    }

    void addMultMatrix(Matrix4<T> mat)
    {
        transformation.back() *= mat;
    }

    void addScaleMatrix(T x, T y, T z)
    {
        addMultMatrix(Matrix4<T>::scale(x, y, z));
    }

    void addRotateXMatrix(T angle)
    {
        addMultMatrix(Matrix4<T>::rotationX(angle));
    }
    void addRotateYMatrix(T angle)
    {
        addMultMatrix(Matrix4<T>::rotationY(angle));
    }
    void addRotateZMatrix(T angle)
    {
        addMultMatrix(Matrix4<T>::rotationZ(angle));
    }

    void addTranslateMatrix(T x, T y, T z)
    {
        addMultMatrix(Matrix4<T>::translation(x, y, z));
    }
    void addTranslateMatrix(Vector3<T> vec)
    {
        addMultMatrix(Matrix4<T>::translation(vec.x, vec.y, vec.z));
    }

    Matrix4<T> getTransformationMatrix()
    {
        return transformation.back();
    }

    Matrix4<T> compute(Matrix4<T> mat)
    {
        for (Matrix4<T> x : transformation)
        {
            mat = x * mat;
        }
        return mat;
    }
    Vector3<T> compute(Vector3<T> vec)
    {
        for (Matrix4<T> x : transformation)
        {
            vec = x * vec;
        }
        return vec;
    }
};

#endif

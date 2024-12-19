#ifndef MATRIX4X4_H
#define MATRIX4X4_H
#include <cmath>
#include "vectorn.h"

class Matrix4x4 {
    float arr[4][4];
public:
    Matrix4x4();
    Matrix4x4(float m[][4]);

    float get(int i, int j);
    void set(int i, int j, float f);

    void setRow(int i, float row[4]);
    void setCol(int j, float col[4]);
    void setMatrix(float m[][4]);

    Matrix4x4 operator+(Matrix4x4& m);
    Matrix4x4 operator*(Matrix4x4& m);
    Vector3<float> operator*(Vector3<float> vec);

    Matrix4x4 operator*(float const& f);

    Matrix4x4 transpose();

    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    void translate(Vector3<float> pos);
    void scale(Vector3<float>scalar);

    friend std::ostream& operator<<(std::ostream& s, Matrix4x4& m);

    static Matrix4x4 identity();
};

#endif // MATRIX4X4_H

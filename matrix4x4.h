#ifndef MATRIX4X4_H
#define MATRIX4X4_H

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

    Matrix4x4 operator*(float const& f);

    Matrix4x4 transpose();

    void print();

    static Matrix4x4 identity();
};

#endif // MATRIX4X4_H

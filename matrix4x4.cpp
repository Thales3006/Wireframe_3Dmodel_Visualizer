#include "matrix4x4.h"
#include <iostream>

Matrix4x4::Matrix4x4() {
    size_t i, j;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            arr[i][j]=0;
}

Matrix4x4::Matrix4x4(float m[][4]) {
    size_t i, j;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            arr[i][j]=m[i][j];
}

float Matrix4x4::get(int i, int j) {
    if(i>=0&&i<4&&j>=0&&j<4)
        return arr[i][j];
    return 0.0;
}

void Matrix4x4::set(int i, int j, float f) {
    if(i>=0&&i<4&&j>=0&&j<4)
        arr[i][j]=f;
}

void Matrix4x4::setRow(int i, float row[4]) {
    size_t j;
    for(j=0;j<4;j++)
        arr[i][j]=row[j];
}

void Matrix4x4::setCol(int j, float col[4]) {
    size_t i;
    for(i=0;i<4;i++)
        arr[i][j]=col[i];
}

void Matrix4x4::setMatrix(float m[][4]) {
    size_t i, j;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            arr[i][j]=m[i][j];
}

Matrix4x4 Matrix4x4::operator+(Matrix4x4& m) {
    size_t i, j;
    Matrix4x4 n;
    for(i=0;i<4;i++) {
        for(j=0;j<4;j++)
            n.set(i,j,get(i,j)+m.get(i,j));
    }
    return n;
};

Matrix4x4 Matrix4x4::operator*(Matrix4x4& m) {
    float temp;
    size_t i, j, k;
    Matrix4x4 n;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++) {
            temp=0.0;
            for(k=0;k<4;k++)
                temp+=get(i,k)*m.get(k,j);
            n.set(i,j,temp);
        }
    return n;
};

Matrix4x4 Matrix4x4::operator*(float const& f) {
    size_t i, j;
    Matrix4x4 n;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            n.set(i,j,f*get(i,j));
    return n;
};

Matrix4x4 Matrix4x4::transpose() {
    size_t i, j;
    Matrix4x4 m;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            m.set(j,i,get(i,j));
    return m;
}

Matrix4x4 Matrix4x4::identity() {
    size_t i;
    Matrix4x4 I;
    for(i=0;i<4;i++)
        I.set(i,i,1.0);
    return I;
}

void Matrix4x4::rotateX(float angle){
    float co = cos(angle);
    float si = sin(angle);
    float a[4][4] = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, co , -si, 0.0},
        {0.0, si , co , 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
    Matrix4x4 mat(a);
    *this = mat * (*this);
}

void Matrix4x4::rotateY(float angle){
    float co = cos(angle);
    float si = sin(angle);
    float a[4][4] = {
        {co , 0.0, si , 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {-si, 0.0, co , 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
    Matrix4x4 mat(a);

    *this = mat * (*this);
}

void Matrix4x4::rotateZ(float angle){
    float co = cos(angle);
    float si = sin(angle);
    float a[4][4] = {
        {co , -si, 0.0, 0.0},
        {si , co , 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
    Matrix4x4 mat(a);

    *this = mat * (*this);
}

void Matrix4x4::translate(Vector3<float> trans) {
    Matrix4x4 id = Matrix4x4::identity();
    id.set(0,3, trans.x);
    id.set(1,3, trans.y);
    id.set(2,3, trans.z);

    Matrix4x4 result = id * (*this);

    *this = result;
}

void Matrix4x4::scale(Vector3<float> scalar){
    Matrix4x4 id = Matrix4x4::identity();
    id.set(0,0, scalar.x);
    id.set(1,1, scalar.y);
    id.set(2,2, scalar.z);

    Matrix4x4 result = id * (*this);

    *this = result;
}

Vector3<float> Matrix4x4::operator*(Vector3<float> vec){
    return Vector3<float>(
        vec.x*arr[0][0] + vec.y*arr[0][1] + vec.z*arr[0][2] + arr[0][3],
        vec.x*arr[1][0] + vec.y*arr[1][1] + vec.z*arr[1][2] + arr[1][3],
        vec.x*arr[2][0] + vec.y*arr[2][1] + vec.z*arr[2][2] + arr[2][3]
        );
}

std::ostream& operator<<(std::ostream& s, Matrix4x4& m) {
    int i, j;
    s << "{";
    for(i=0;i<4;i++) {
        if(i)
            s << ",";
        s << "\n  { ";
        for(j=0;j<4;j++) {
            if(j)
                s << ", ";
            s << m.get(i,j);
        }
        s << " }";
    }
    s << "\n}";

    return s;
}

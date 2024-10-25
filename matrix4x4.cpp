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

void Matrix4x4::print() {
    char i, j;
    std::cout << "{";
    for(i=0;i<4;i++) {
        if(i)
            std::cout << ",";
        std::cout << "\n  { ";
        for(j=0;j<4;j++) {
            if(j)
                std::cout << ", ";
            std::cout << get(i,j);
        }
        std::cout << " }";
    }
    std::cout << "\n}\n";
}

Matrix4x4 Matrix4x4::identity() {
    size_t i;
    Matrix4x4 I;
    for(i=0;i<4;i++)
        I.set(i,i,1.0);
    return I;
}

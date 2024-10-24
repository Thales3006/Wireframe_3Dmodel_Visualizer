#include "matrix.h"
#include <iostream>

template <typename T> Matrix<T>::Matrix(size_t rows, size_t cols) : dimensions{rows, cols} {
    int i;
    arr=(T**)malloc(rows*sizeof(T*));
    for(i=0;i<rows;i++)
        arr[i]=(T*)calloc(cols,sizeof(T));
}

template <typename T> template <typename Fn> Matrix<T>::Matrix(size_t rows, size_t cols, Fn const& func) : dimensions{rows, cols} {
    int i, j;
    arr=(T**)malloc(rows*sizeof(T*));
    for(i=0;i<rows;i++) {
        arr[i]=(T*)malloc(cols*sizeof(T));
        for(j=0;j<cols;j++)
            arr[i][j]=func(i,j);
    }
}

template <typename T> Matrix<T>::~Matrix() {
    int i;
    for(i=0;i<dimensions[0];i++)
        free(arr[i]);
    free(arr);
    arr=NULL;
}

template <typename T> void Matrix<T>::set(int i, int j, T val) {
    if(i>=0&&i<dimensions[0]&&j>=0&&j<dimensions[1])
        arr[i][j]=val;
}

template <typename T> std::optional<T> Matrix<T>::get(int i, int j) {
    if(i>=0&&i<dimensions[0]&&j>=0&&j<dimensions[1])
        return arr[i][j];
    return {};
}

template <typename T> std::optional<Matrix<T>> Matrix<T>::operator+(Matrix<T>& m) {
    size_t r=dimensions[0], c=dimensions[1];
    int i, j;
    if(r!=m.dimensions[0]||c!=m.dimensions[1])
        return {};
    Matrix<T> n(r,c);
    for(i=0;i<r;i++)
        for(j=0;j<c;j++)
            n.set(i,j,*(get(i,j))+*(m.get(i,j)));
    return n;
}

template <typename T> std::optional<Matrix<T>> Matrix<T>::operator*(Matrix<T>& m) {
    size_t r=dimensions[0], c=m.dimensions[1], k0=dimensions[1];
    int i, j, k;
    T temp;
    if(k0!=m.dimensions[0])
        return {};
    Matrix n(r, c);
    for(i=0;i<r;i++)
        for(j=0;j<c;j++) {
            temp=0;
            for(k=0;k<k0;k++)
                temp+=get(i,k)*m.get(k,j);
            n.set(i,j,temp);
        }
    return n;
}

template <typename T> Matrix<T> Matrix<T>::operator*(T a) {
    int i,j;
    Matrix<T> m(dimensions[0], dimensions[1]);
    for(i=0;i<dimensions[0];i++)
        for(j=0;j<dimensions[1];j++)
            m.set(i,j,arr[i][j]*a);
    return m;
}

template <typename T> void Matrix<T>::print() {
    int i, j;
    std::cout << "{";
    for(i=0;i<dimensions[0];i++) {
        if(i)
            std::cout << ",";
        std::cout << "\n  { ";
        for(j=0;j<dimensions[1];j++) {
            if(j)
                std::cout << ", ";
            std::cout << arr[i][j];
        }
        std::cout << " }";
    }
    std::cout << "\n}\n";
}
#include <cmath>

// Matrix::Matrix(std::vector<std::vector<double>>& fs) {
    // int i, j;
    // for(i=0;i<4;i++)
        // for(j=0;j<4;j++)
            // vals[i][j]=fs[i][j];
// }

Matrix::Matrix() {
    int i, j;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            vals[i][j]=0;
}

void Matrix::set(int i, int j, double x) {
    if(i>=0&&i<4&&j>=0&&j<4)
        vals[i][j]=x;
}

double Matrix::get(int i, int j) {
    return i>=0&&i<4&&j>=0&&j<4 ? vals[i][j] : NAN;
}

void Matrix::print() {
    int i, j;
    std::cout << "{";
    for(i=0;i<4;i++) {
        std::cout << "\n  { ";
        for(j=0;j<4;j++) {
            if(j)
                std::cout << ", ";
            std::cout << vals[i][j];
        }
        std::cout << " }";
    }
    std::cout << "\n}\n";
}

void Matrix::mult(double const& n) {
    int i, j;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            vals[i][j]*=n;
}

void Matrix::mult(Matrix const& m) {
    int i, j, k;
    double s;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++) {
            s=0;
            for(k=0;k<4;k++) {
                s+=vals[i][k]*m[k][j];
            }
            vals
        }
}

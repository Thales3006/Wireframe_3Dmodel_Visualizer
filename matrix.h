#ifndef MATRIX_H
#define MATRIX_H
#include <optional>
#include <cmath>

template <typename T> class Matrix {
    T** arr;
    public:
        const size_t dimensions[2];

        Matrix(size_t rows, size_t cols);
        template <typename Fn> Matrix(size_t rows, size_t cols, Fn const& func);
        ~Matrix();

        Matrix<T> identity(size_t n);

        void set(int i, int j, T val);
        std::optional<T> get(int i, int j);
        void rotate(float angle, int plane1, int plane2);


        std::optional<Matrix<T>> operator+(Matrix<T>& m);
        std::optional<Matrix<T>> operator*(Matrix<T>& m);
        Matrix<T> operator*(T a);

        void print();
};

#endif // MATRIX_H

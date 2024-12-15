#ifndef VECTORN_H
#define VECTORN_H

#include <iostream>

template <typename T>
class Vector2 {
public:
    T x, y;

    // Construtor padrão
    Vector2() : x(0), y(0) {}

    // Construtor parametrizado
    Vector2(T x, T y) : x(x), y(y) {}

    // Operadores
    Vector2<T> operator+(const Vector2<T>& other) const {
        return Vector2<T>(x + other.x, y + other.y);
    }

    Vector2<T> operator-(const Vector2<T>& other) const {
        return Vector2<T>(x - other.x, y - other.y);
    }

    Vector2<T>& operator+=(const Vector2<T>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2<T>& operator-=(const Vector2<T>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};

template <typename T>
class Vector3 {
public:
    T x, y, z;

    Vector3() : x(0), y(0), z(0) {}

    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    T length() const {
        return sqrt(x*x + y*y + z*z);
    }

    Vector3<T> operator+(const Vector3<T>& other) const {
        return Vector3<T>(x + other.x, y + other.y, z + other.z);
    }

    Vector3<T> operator-(const Vector3<T>& other) const {
        return Vector3<T>(x - other.x, y - other.y, z - other.z);
    }

    Vector3<T> operator*(const T scalar) const {
        return Vector3<T>(x*scalar, y*scalar, z*scalar);
    }

    Vector3<T> operator/(const T scalar) const {
        return Vector3<T>(x/scalar, y/scalar, z/scalar);
    }

    T operator*(const Vector3<T>& other) const {
        return x*other.x + y*other.y + z*other.z;
    }

    Vector3<T> operator^(const Vector3<T>& other) const {
        return Vector3<T>(y*other.z - z*other.y,
                          -x*other.z + z*other.x,
                          x*other.y - y*other.x);
    }

    Vector3<T> operator-() const {
        return Vector3<T>(-x, -y, -z);
    }

    Vector3<T>& operator+=(const Vector3<T>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3<T>& operator-=(const Vector3<T>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3<T> normalize(){
        T mag = this->length();
        if(mag==0)
            return Vector3<T>(0.0,0.0,0.0);
        return Vector3<T>(x/mag,y/mag,z/mag);
    }

    T angle(Vector3<T> vec){
        return (x > 0? 1.0:-1.0) * acos( (*this) * vec / this->length());
    }

};

template <typename T>
class Vector4 {
public:
    T x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(0) {}

    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    Vector4<T> operator+(const Vector4<T>& other) const {
        return Vector4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Vector4<T> operator-(const Vector4<T>& other) const {
        return Vector4<T>(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    Vector4<T>& operator+=(const Vector4<T>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    Vector4<T>& operator-=(const Vector4<T>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& s, const Vector2<T>& v) {
    s << "<" << v.x << ", " << v.y << ">";
    return s;
}

template <typename T>
std::ostream& operator<<(std::ostream& s, const Vector3<T>& v) {
    s << "<" << v.x << ", " << v.y << ", " << v.z << ">";
    return s;
}

template <typename T>
std::ostream& operator<<(std::ostream& s, const Vector4<T>& v) {
    s << "<" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ">";
    return s;
}
#endif // VECTORN_H

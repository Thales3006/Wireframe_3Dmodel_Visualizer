#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QMainWindow>
#include <iostream>
#include "matrix4x4.h"
#include "vectorn.h"

class Geometry{
public:
    Matrix4x4 matrix = Matrix4x4::identity();

    virtual ~Geometry() {}

    virtual void draw(QImage& canvas) {
        Q_UNUSED(canvas);
        std::cout << "Rewrite draw function!" << std::endl;
    }

    virtual std::vector<std::unique_ptr<Geometry>> drawable() {
        std::vector<std::unique_ptr<Geometry>> v;
        std::cout << "Rewrite drawable function!" << std::endl;
        return v;
    }

    virtual std::unique_ptr<Geometry> multiply(Matrix4x4& matrix){
        Q_UNUSED(matrix);
        std::cout << "Rewrite multiply function!" << std::endl;
        return NULL;
    }

    virtual Vector3<float> mean(){
        std::cout << "Rewrite mean function!" << std::endl;
        return Vector3<float>(0,0,0);
    }  

    virtual void print() {
        std::cout << "Rewrite print function!" << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& s, Geometry& g);

    virtual std::unique_ptr<Geometry> divideZ(){
        std::cout << "Rewrite divide function!" << std::endl;
        return NULL;
    }
};

class Point : public Geometry
{
public:
    Vector3<float> pos;
    unsigned char r, g, b;

    Point(float x0, float y0, float z0, unsigned char r0, unsigned char g0, unsigned char b0);
    Point(Vector3<float> p);
    Point(Vector3<float> p, unsigned char r0, unsigned char g0, unsigned char b0);

    void draw(QImage& canvas) override;

    std::unique_ptr<Point> drawablePoint();
    std::vector<std::unique_ptr<Geometry>> drawable() override;

    std::unique_ptr<Geometry> multiply(Matrix4x4& matrix) override;

    Vector3<float> mean() override;

    bool operator==(const Point& p);

    char getRC();

    virtual void print() override;
    friend std::ostream& operator<<(std::ostream& s, const Point& p);
    virtual std::unique_ptr<Geometry> divideZ() override;
};

class Line : public Geometry
{
public:
    Point p1;
    Point p2;

    Line(Point p10, Point p20);

    void draw(QImage& canvas) override;

    std::unique_ptr<Line> drawableLine();
    std::vector<std::unique_ptr<Geometry>> drawable() override;

    std::unique_ptr<Geometry> multiply(Matrix4x4& matrix) override;

    Vector3<float> mean() override;

    bool operator==(const Line& l);
    bool operator>=(const Line& l);

    virtual void print() override;
    friend std::ostream& operator<<(std::ostream& s, const Line& l);
    std::unique_ptr<Geometry> divideZ() override;
};

class Polygon : public Geometry
{
public:
    Line l1;
    Line l2;
    Line l3;

    Polygon(Point q1, Point q2, Point q3);
    Polygon(Line q1, Line q2, Line q3);

    std::vector<std::unique_ptr<Line>> drawablePolygon();
    std::vector<std::unique_ptr<Geometry>> drawable() override;

    void rotate3d(Vector3<float> rot);

    void draw(QImage& canvas) override;
    void fill(QImage& canvas);

    std::unique_ptr<Geometry> multiply(Matrix4x4& matrix) override;

    Vector3<float> mean() override;

    virtual void print() override;
    friend std::ostream& operator<<(std::ostream& s, const Polygon& p);
    bool operator==(const Polygon& p);
    bool operator!=(const Polygon& p);
    std::unique_ptr<Geometry> divideZ() override;
};

#endif // GEOMETRY_H

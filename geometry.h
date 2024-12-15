#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QMainWindow>
#include <iostream>
#include "matrix4x4.h"
#include "vectorn.h"
#include <optional>

class Geometry{
public:
    Matrix4x4 matrix = Matrix4x4::identity();

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

};

class Point : public Geometry
{
public:
    float x, y, z;
    unsigned char r, g, b;

    Point(float x0, float y0, float z0, unsigned char r0, unsigned char g0, unsigned char b0);
    Point(Vector3<float> p);

    void draw(QImage& canvas) override;

    std::unique_ptr<Point> drawablePoint();
    std::vector<std::unique_ptr<Geometry>> drawable() override;

    std::unique_ptr<Geometry> multiply(Matrix4x4& matrix) override;

    Vector3<float> mean() override;

    bool operator==(Point& p);

    char getRC();
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

    bool operator==(Line& l);
};

class Polygon : public Geometry
{
public:
    Line l1;
    Line l2;
    Line l3;

    Polygon(Point q1, Point q2, Point q3);

    std::vector<std::unique_ptr<Line>> drawablePolygon();
    std::vector<std::unique_ptr<Geometry>> drawable() override;

    void rotate3d(Vector3<float> rot);

    void draw(QImage& canvas) override;
    void fill(QImage& canvas);

    std::unique_ptr<Geometry> multiply(Matrix4x4& matrix) override;

    Vector3<float> mean() override;

};

#endif // GEOMETRY_H

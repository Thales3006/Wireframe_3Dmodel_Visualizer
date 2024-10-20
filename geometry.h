#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QMainWindow>
#include <iostream>

class Geometry{
public:
    virtual void draw(QImage& canvas){
        Q_UNUSED(canvas);
        std::cout << "Rewrite this function!" << std::endl;
    }
};

class Point : public Geometry
{
public:
    float x, y, z;
    unsigned char r, g, b;

    Point(float x0, float y0, float z0, unsigned char r0, unsigned char g0, unsigned char b0);

    void draw(QImage& canvas);
};

class Line : public Geometry
{
public:
    Point p1;
    Point p2;

    Line(Point p10, Point p20);

    void draw(QImage& canvas);
};

class Polygon : public Geometry
{
public:
    Point p1;
    Point p2;
    Point p3;

    Polygon(Point q1, Point q2, Point q3);

    void draw(QImage& canvas);
    void drawHollow(QImage& canvas);

};

#endif // GEOMETRY_H

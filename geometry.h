#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QMainWindow>

class Point
{
public:
    float x, y, z;
    unsigned char r, g, b;

    Point(float x0, float y0, float z0, unsigned char r0, unsigned char g0, unsigned char b0);

    void draw(QImage& canvas);
};

class Line
{
public:
    Point p1;
    Point p2;

    Line(Point p10, Point p20);

    void draw(QImage& canvas);
};

/* Não implementados ainda
class Polygon
{
public:
    Polygon(Point p1, Point p2, Point p3);
};
*/

#endif // GEOMETRY_H

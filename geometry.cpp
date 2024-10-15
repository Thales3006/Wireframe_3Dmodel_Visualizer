#include "geometry.h"
#include <math.h>

Point::Point(float x0, float y0, float z0, unsigned char r0, unsigned char g0, unsigned char b0){
    x = x0;
    y = y0;
    z = z0;

    r = r0;
    g = g0;
    b = b0;
}

void Point::draw(QImage& canvas){
    canvas.setPixel(x,y, qRgb(r,g,b));
}

Line::Line(Point p10, Point p20) : p1(p10.x,p10.y,p10.z, p10.r,p10.g,p10.b), p2(p20.x,p20.y,p20.z, p20.r,p20.g,p20.b){
}

void Line::draw(QImage& canvas){
    Point p(p1.x-p2.x, p1.y-p2.y, 0, 0,0,0);
    float dist = 1/sqrt(pow(p.x,2)+pow(p.y,2));
    for(float t=0; t<=1; t+=dist){
        p.x = (1-t)*p1.x + t*p2.x;
        p.y = (1-t)*p1.y + t*p2.y;
        p.z = (1-t)*p1.z + t*p2.z;

        p.r = (1-t)*p1.r + t*p2.r;
        p.g = (1-t)*p1.g + t*p2.g;
        p.b = (1-t)*p1.b + t*p2.b;

        p.draw(canvas);
    }
}
/*
Polygon::Polygon(){

}
*/


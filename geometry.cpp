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
        //coords transition
        p.x = (1-t)*p1.x + t*p2.x;
        p.y = (1-t)*p1.y + t*p2.y;
        p.z = (1-t)*p1.z + t*p2.z;
        //rgb transition
        p.r = (1-t)*p1.r + t*p2.r;
        p.g = (1-t)*p1.g + t*p2.g;
        p.b = (1-t)*p1.b + t*p2.b;

        p.draw(canvas);
    }
}

Polygon::Polygon(Point q1, Point q2, Point q3) : p1(q1.x,q1.y,q1.z,q1.r,q1.g,q1.b), p2(q2.x,q2.y,q2.z,q2.r,q2.g,q2.b), p3(q3.x,q3.y,q3.z,q3.r,q3.g,q3.b){

}

void Polygon::draw(QImage& canvas){
    Line l(p1,p2);
    float dist1 = 1/sqrt(pow(p1.x-p3.x,2)+pow(p1.y-p3.y,2));
    float dist2 = 1/sqrt(pow(p1.x-p3.x,2)+pow(p1.y-p3.y,2));
    float dist = dist1 > dist2? dist1 : dist2;

    for(float t=0; t<=1; t+=dist){
        //coords transition
        l.p1.x = (1-t)*p1.x + t*p3.x;
        l.p2.x = (1-t)*p2.x + t*p3.x;

        l.p1.y = (1-t)*p1.y + t*p3.y;
        l.p2.y = (1-t)*p2.y + t*p3.y;

        l.p1.z = (1-t)*p1.z + t*p3.z;
        l.p2.z = (1-t)*p2.z + t*p3.z;

        //rgb transition
        l.p1.r = (1-t)*p1.r + t*p3.r;
        l.p2.r = (1-t)*p2.r + t*p3.r;

        l.p1.g = (1-t)*p1.g + t*p3.g;
        l.p2.g = (1-t)*p2.g + t*p3.g;

        l.p1.b = (1-t)*p1.b + t*p3.b;
        l.p2.b = (1-t)*p2.b + t*p3.b;

        l.draw(canvas);
    }
}

void Polygon::drawHollow(QImage& canvas){
    Line l1(p1,p2);
    Line l2(p2,p3);
    Line l3(p3,p1);
    l1.draw(canvas);
    l2.draw(canvas);
    l3.draw(canvas);
}

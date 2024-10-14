#include "geometry.h"

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

/*
Line::Line(){

}

Polygon::Polygon(){

}
*/


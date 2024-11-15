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
    int nY = canvas.height()-y;
    if(x >= 0 && x < canvas.width() && nY >= 0 && nY < canvas.height())
        canvas.setPixel(x,nY, qRgb(r,g,b));
}

std::unique_ptr<Geometry> Point::drawable() {
    return this->getRC() == 0 ? std::make_unique<Point>(x, y, z, r, g, b) : NULL;
}

std::unique_ptr<Geometry> Point::multiply(Matrix4x4& matrix){
    return std::make_unique<Point>(
        x*matrix.get(0,0) + y*matrix.get(0,1) + z*matrix.get(0,2) + matrix.get(0,3),
        x*matrix.get(1,0) + y*matrix.get(1,1) + z*matrix.get(1,2) + matrix.get(1,3),
        x*matrix.get(2,0) + y*matrix.get(2,1) + z*matrix.get(2,2) + matrix.get(2,3),
        r,
        g,
        b);
}

Vector3<float> Point::mean(){
    return Vector3<float>(x,y,z);
}

char Point::getRC() {
    return (char)((x<-1 ? 8 : (x>1 ? 4 : 0)) | (y<-1 ? 2 : (y>1 ? 1 : 0)));
}

Line::Line(Point p10, Point p20) : p1(p10.x,p10.y,p10.z, p10.r,p10.g,p10.b), p2(p20.x,p20.y,p20.z, p20.r,p20.g,p20.b){
}

void Line::draw(QImage& canvas){
    Point p(p1.x-p2.x, p1.y-p2.y, 0, 0,0,0);
    float dist = 1/sqrt(pow(p.x,2)+pow(p.y,2));

    float t = 0;
    while(t <= 1){
        //coords transition
        p.x = (1-t)*p1.x + t*p2.x;
        p.y = (1-t)*p1.y + t*p2.y;
        p.z = (1-t)*p1.z + t*p2.z;
        //rgb transition
        p.r = (1-t)*p1.r + t*p2.r;
        p.g = (1-t)*p1.g + t*p2.g;
        p.b = (1-t)*p1.b + t*p2.b;

        p.draw(canvas);
        t += dist;
    }
}

std::unique_ptr<Geometry> Line::drawable() {
    char rc1=p1.getRC(), rc2=p2.getRC();
    if((rc1&rc2)!=0)
        return NULL;
    else if(rc1==rc2) // rc1==0 && rc2==0
        return std::make_unique<Line>(
            Point(p1.x, p1.y, p1.z, p1.r, p1.g, p1.b),
            Point(p2.x, p2.y, p2.z, p2.r, p2.g, p2.b));
    Vector3<float> v1 = p1.mean();
    Vector3<float> v2 = p2.mean();
    Vector3<float> d;
    float k;

    d = (v2-v1).normalize();

    // v1 = p + d*k;

    // v1.x = p.x + d.x*k;
    // v1.y = p.y + d.y*k;

    // 1 = p.x + d.x*k;
    // v1.y = p.y + d.y*k;
    // (1-p.x)/d.x = k;

    // v1.x = p.x + d.x*k;
    // 1 = p.y + d.y*k;
    // (1-p.y)/d.y = k;
    if(rc1!=0) {
        if(rc1&1)
            k=(1-v1.y)/d.y;
        else if(rc1&2)
            k=(-1-v1.y)/d.y;
        else
            k=0;
        v1 += d*k;
        if(v1.x>1||v1.x<-1) {
            if(rc1&4)
                k=(1-v1.x)/d.x;
            else if(rc1&8)
                k=(-1-v1.x)/d.x;
            else
                k=0;
            v1 += d*k;
            if(v1.y>1||v1.y<-1)
                return NULL;
        }
    }
    if(rc2!=0) {
        if(rc2&1)
            k=(1-v2.y)/d.y;
        else if(rc2&2)
            k=(-1-v2.y)/d.y;
        else
            k=0;
        v2 += d*k;
        if(v2.x>1||v2.x<-1) {
            if(rc2&4)
                k=(1-v2.x)/d.x;
            else if(rc2&8)
                k=(-1-v2.x)/d.x;
            else
                k=0;
            v2 += d*k;
            if(v2.y>1||v2.y<-1)
                return NULL;
        }
    }

    return std::make_unique<Line>(
        Point(v1.x, v1.y, v1.z, p1.r, p1.g, p1.b),
        Point(v2.x, v2.y, v2.z, p2.r, p2.g, p2.b)
    );
}

std::unique_ptr<Geometry> Line::multiply(Matrix4x4& matrix){
    std::unique_ptr<Geometry> newP1 = p1.multiply(matrix);
    std::unique_ptr<Geometry> newP2 = p2.multiply(matrix);

    return std::make_unique<Line>(*(Point*)newP1.get(), *(Point*)newP2.get() );
}

Vector3<float> Line::mean(){
    return Vector3<float>( (p1.x+p2.x)/2, (p1.y+p2.y)/2, (p1.z+p2.z)/2);
}

Polygon::Polygon(Point q1, Point q2, Point q3) : p1(q1.x,q1.y,q1.z,q1.r,q1.g,q1.b), p2(q2.x,q2.y,q2.z,q2.r,q2.g,q2.b), p3(q3.x,q3.y,q3.z,q3.r,q3.g,q3.b){

}

std::unique_ptr<Geometry> Polygon::drawable() {
    char rc1=p1.getRC(), rc2=p2.getRC(), rc3=p3.getRC();
    if((rc1&rc2)||(rc1&rc3)||(rc2&rc3))
        return NULL;
    return std::make_unique<Polygon>(
        Point(p1.x, p1.y, p1.z, p1.r, p1.g, p1.b),
        Point(p2.x, p2.y, p2.z, p2.r, p2.g, p2.b),
        Point(p3.x, p3.y, p3.z, p3.r, p3.g, p3.b)
        );
}

void Polygon::rotate3d(Vector3<float> rot){
    Matrix4x4 m = Matrix4x4::identity();
    m.set(0,0,p1.x);
    m.set(0,1,p1.y);
    m.set(0,2,p1.z);

    m.rotate(rot.x, 1,2);
    m.rotate(rot.y, 0,2);
    m.rotate(rot.z, 0,1);

    p1.x = m.get(0,0);
    p1.y = m.get(0,1);
    p1.x = m.get(0,2);
}

void Polygon::draw(QImage& canvas){
    Line l(p1,p2);
    //l = *(Line*)(l.drawable()).get();
    float dist1 = 1/sqrt(pow(p1.x-p3.x,2)+pow(p1.y-p3.y,2));
    float dist2 = 1/sqrt(pow(p1.x-p3.x,2)+pow(p1.y-p3.y,2));
    float dist = (dist1 > dist2? dist1 : dist2) * 0.95;

    float t = 0;
    while(t <= 1){
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
        t += dist;
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

std::unique_ptr<Geometry> Polygon::multiply(Matrix4x4& matrix){
    std::unique_ptr<Geometry> newP1 = p1.multiply(matrix);
    std::unique_ptr<Geometry> newP2 = p2.multiply(matrix);
    std::unique_ptr<Geometry> newP3 = p3.multiply(matrix);

    return std::make_unique<Polygon>(*(Point*)newP1.get(), *(Point*)newP2.get(), *(Point*)newP3.get() );
}

 Vector3<float> Polygon::mean(){
    return Vector3<float>( (p1.x+p2.x+p3.x)/3, (p1.y+p2.y+p3.y)/3, (p1.z+p2.z+p3.z)/3);
}

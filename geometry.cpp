#include "geometry.h"
#include <math.h>
#include <iostream>
#include <optional>

std::ostream& operator<<(std::ostream& s, Geometry& g) {
    g.print();
    return s;
}

Point::Point(float x0, float y0, float z0, unsigned char r0, unsigned char g0, unsigned char b0){
    x = x0;
    y = y0;
    z = z0;

    r = r0;
    g = g0;
    b = b0;
}

Point::Point(Vector3<float> p){
    x = p.x;
    y = p.y;
    z = p.z;

    r = 0;
    g = 0;
    b = 0;
}

void Point::draw(QImage& canvas){
    int nY = canvas.height()-y;
    if(x >= 0 && x < canvas.width() && nY >= 0 && nY < canvas.height())
        canvas.setPixel(x,nY, qRgb(r,g,b));
}

std::unique_ptr<Point> Point::drawablePoint() {
    return this->getRC() == 0 ? std::make_unique<Point>(*this) : NULL;
}

std::vector<std::unique_ptr<Geometry>> Point::drawable() {
    std::vector<std::unique_ptr<Geometry>> v;
    std::unique_ptr<Point> p = this->drawablePoint();
    if(p!=NULL)
        v.push_back(std::move(p));
    return v;
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

bool Point::operator==(Point& p) {
    return x==p.x && y==p.y && z==p.z;
}

char Point::getRC() {
    return (char)((x<-1 ? 8 : (x>1 ? 4 : 0)) | (y<-1 ? 2 : (y>1 ? 1 : 0)));
}

void Point::print() {
    std::cout << this;
}

std::ostream& operator<<(std::ostream& s, const Point& p) {
    s << "( " << p.x << ", " << p.y << ", " << p.z;
    s << " : " << (int)p.r << ", " << (int)p.g << ", " << (int)p.b << " )";
    return s;
}

Line::Line(Point p10, Point p20) :
    p1(p10),
    p2(p20)
{ }

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

std::unique_ptr<Line> Line::drawableLine() {
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

    auto interceptBorder = [](Vector3<float> dir, Vector3<float> v, char rc) {
        float k;
        if(rc==0)
            return std::optional<Vector3<float>>{v};
        else if(((rc&3) && dir.y==0) || ((rc&12) && dir.x==0))
            return std::optional<Vector3<float>>{};

        dir=dir.normalize();

        if(rc&1)
            k=(1-v.y)/dir.y;
        else if(rc&2)
            k=(-1-v.y)/dir.y;
        else
            k=0;

        v+=dir*k;

        if(v.x>1)
            k=(1-v.x)/dir.x;
        else if(v.x<-1)
            k=(-1-v.x)/dir.x;
        else
            k=0;

        v+=dir*k;

        if(v.y>1||v.y<-1)
            return std::optional<Vector3<float>>{};
        else
            return std::optional<Vector3<float>>{v};
    };

    d = (v2-v1).normalize();

    std::optional<Vector3<float>> maybeV1=interceptBorder(d, v1, rc1);
    if(!maybeV1)
        return NULL;

    std::optional<Vector3<float>> maybeV2=interceptBorder(d, v2, rc2);
    if(!maybeV2)
        return NULL;

    v1 = std::move(*maybeV1);
    v2 = std::move(*maybeV2);

    return std::make_unique<Line>(
        Point(v1.x, v1.y, v1.z, p1.r, p1.g, p1.b),
        Point(v2.x, v2.y, v2.z, p2.r, p2.g, p2.b)
    );
}

std::vector<std::unique_ptr<Geometry>> Line::drawable() {
    std::vector<std::unique_ptr<Geometry>> v;
    std::unique_ptr<Line> l = this->drawableLine();
    if(l!=NULL) {
        if(l->p1==l->p2)
            v.push_back(std::make_unique<Point>(l->p1));
        else
            v.push_back(std::move(l));
    }
    return v;
}

std::unique_ptr<Geometry> Line::multiply(Matrix4x4& matrix){
    std::unique_ptr<Geometry> newP1 = p1.multiply(matrix);
    std::unique_ptr<Geometry> newP2 = p2.multiply(matrix);

    return std::make_unique<Line>(*(Point*)newP1.get(), *(Point*)newP2.get() );
}

Vector3<float> Line::mean(){
    return Vector3<float>( (p1.x+p2.x)/2, (p1.y+p2.y)/2, (p1.z+p2.z)/2);
}

bool Line::operator==(Line& l) {
    return p1==l.p1 && p2==l.p2;
}

void Line::print() {
    std::cout << this;
}

std::ostream& operator<<(std::ostream& s, const Line& l) {
    s << "{ p1: " << l.p1 << ", p2: " << l.p2 << " }";
    return s;
}

Polygon::Polygon(Point q1, Point q2, Point q3) :
    l1(q1, q2),
    l2(q2, q3),
    l3(q3, q1)
{ }

std::vector<std::unique_ptr<Line>> Polygon::drawablePolygon() {
    std::vector<std::unique_ptr<Line>> v;
    std::unique_ptr<Line> newL1 = l1.drawableLine();
    std::unique_ptr<Line> newL2 = l2.drawableLine();
    std::unique_ptr<Line> newL3 = l3.drawableLine();
    if(newL1!=NULL)
        v.push_back(std::move(newL1));
    if(newL2!=NULL && newL2!=newL1)
        v.push_back(std::move(newL2));
    if(newL3!=NULL && newL3!=newL2 && newL3!=newL1)
        v.push_back(std::move(newL3));
    return v;
}

std::vector<std::unique_ptr<Geometry>> Polygon::drawable() {
    std::vector<std::unique_ptr<Line>> vl = this->drawablePolygon();
    std::vector<std::unique_ptr<Geometry>> v;
    for(auto& line : vl)
        v.push_back(std::move(line));
    return v;
}

void Polygon::rotate3d(Vector3<float> rot){
    Matrix4x4 m = Matrix4x4::identity();
    m.set(0,0,l1.p1.x);
    m.set(0,1,l1.p1.y);
    m.set(0,2,l1.p1.z);

    m.rotate(rot.x, 1,2);
    m.rotate(rot.y, 0,2);
    m.rotate(rot.z, 0,1);

    l1.p1.x = m.get(0,0);
    l1.p1.y = m.get(0,1);
    l1.p1.x = m.get(0,2);
}

void Polygon::fill(QImage& canvas){
    Line l(l1.p1,l2.p1);
    float dist1 = 1/sqrt(pow(l1.p1.x-l3.p1.x,2)+pow(l1.p1.y-l3.p1.y,2));
    float dist2 = 1/sqrt(pow(l2.p1.x-l3.p1.x,2)+pow(l2.p1.y-l3.p1.y,2));
    float dist = (dist1 > dist2 ? dist1 : dist2) * 0.95;
    float t = 0;
    while(t <= 1){
        //coords transition
        l.p1.x = (1-t)*l1.p1.x + t*l3.p1.x;
        l.p2.x = (1-t)*l2.p1.x + t*l3.p1.x;

        l.p1.y = (1-t)*l1.p1.y + t*l3.p1.y;
        l.p2.y = (1-t)*l2.p1.y + t*l3.p1.y;

        l.p1.z = (1-t)*l1.p1.z + t*l3.p1.z;
        l.p2.z = (1-t)*l2.p1.z + t*l3.p1.z;

        //rgb transition
        l.p1.r = (1-t)*l1.p1.r + t*l3.p1.r;
        l.p2.r = (1-t)*l2.p1.r + t*l3.p1.r;

        l.p1.g = (1-t)*l1.p1.g + t*l3.p1.g;
        l.p2.g = (1-t)*l2.p1.g + t*l3.p1.g;

        l.p1.b = (1-t)*l1.p1.b + t*l3.p1.b;
        l.p2.b = (1-t)*l2.p1.b + t*l3.p1.b;


        l.draw(canvas);

        t += dist;
    }
}

void Polygon::draw(QImage& canvas){
    l1.draw(canvas);
    l2.draw(canvas);
    l3.draw(canvas);
}

std::unique_ptr<Geometry> Polygon::multiply(Matrix4x4& matrix){
    Line line1 = *dynamic_cast<Line*>(l1.multiply(matrix).get());
    Line line2 = *dynamic_cast<Line*>(l2.multiply(matrix).get());
    Line line3 = *dynamic_cast<Line*>(l3.multiply(matrix).get());

    return std::make_unique<Polygon>(line1.p1, line2.p1, line3.p1 );
}

 Vector3<float> Polygon::mean(){
    return Vector3<float>( (l1.p1.x+l2.p1.x+l3.p1.x)/3, (l1.p1.y+l2.p1.y+l3.p1.y)/3, (l1.p1.z+l2.p1.z+l3.p1.z)/3);
}

 void Polygon::print() {
     std::cout << this;
 }

 std::ostream& operator<<(std::ostream& s, const Polygon& p) {
     s << "{ l1: " << p.l1 << ", l2: " << p.l2 << ", l3: " << p.l3 << " }";
     return s;
 }

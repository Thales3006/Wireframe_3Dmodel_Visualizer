#include "geometry.h"
#include <math.h>
#include <iostream>
#include <optional>

std::ostream& operator<<(std::ostream& s, Geometry& g) {
    g.print();
    return s;
}

Point::Point(float x0, float y0, float z0, unsigned char r0, unsigned char g0, unsigned char b0){
    pos.x = x0;
    pos.y = y0;
    pos.z = z0;

    r = r0;
    g = g0;
    b = b0;
}

Point::Point(Vector3<float> p){
    pos = p;

    r = 0;
    g = 0;
    b = 0;
}

Point::Point(Vector3<float> p, unsigned char r0, unsigned char g0, unsigned char b0){
    pos = p;

    r = r0;
    g = g0;
    b = b0;
}

void Point::draw(QImage& canvas){
    int nY = canvas.height()-pos.y;
    if(pos.x >= 0 && pos.x < canvas.width() && nY >= 0 && nY < canvas.height() && pos.z >= 0)
        canvas.setPixel(pos.x, nY, qRgb(r,g,b));
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
    return std::make_unique<Point>(matrix*pos,r,g,b);
}

Vector3<float> Point::mean(){
    return pos;
}

bool Point::operator==(const Point& p) {
    return pos == p.pos;
}

char Point::getRC() {
    return (char)((pos.x<-1 ? 8 : (pos.x>1 ? 4 : 0)) | (pos.y<-1 ? 2 : (pos.y>1 ? 1 : 0)));
}

void Point::print() {
    std::cout << this;
}

std::ostream& operator<<(std::ostream& s, const Point& p) {
    s << "( " << p.pos.x << ", " << p.pos.y << ", " << p.pos.z;
    s << " : " << (int)p.r << ", " << (int)p.g << ", " << (int)p.b << " )";
    return s;
}

std::unique_ptr<Geometry> Point::divideZ(){
    Point p(pos/pos.z, r,g,b);
    return std::make_unique<Point>(p);
}

Line::Line(Point p10, Point p20) :
    p1(p10),
    p2(p20)
{ }

void Line::draw(QImage& canvas){
    Point p(p1.pos.x-p2.pos.x, p1.pos.y-p2.pos.y, 0, 0,0,0);
    float dist = 1/p.pos.length();

    float t = 0;
    while(t <= 1){
        //coords transition
        p.pos.x = (1-t)*p1.pos.x + t*p2.pos.x;
        p.pos.y = (1-t)*p1.pos.y + t*p2.pos.y;
        p.pos.z = (1-t)*p1.pos.z + t*p2.pos.z;
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
            Point(p1.pos, p1.r, p1.g, p1.b),
            Point(p2.pos, p2.r, p2.g, p2.b));
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
    Point newP1 = *dynamic_cast<Point*>(p1.multiply(matrix).get());
    Point newP2 = *dynamic_cast<Point*>(p2.multiply(matrix).get());;

    return std::make_unique<Line>(newP1, newP2);
}

Vector3<float> Line::mean(){
    return (p1.pos+p2.pos)/2;
}

bool Line::operator==(const Line& l) {
    return p1==l.p1 && p2==l.p2;
}

bool Line::operator>=(const Line& l) {
    return p1==l.p1 && p2==l.p2;
}

void Line::print() {
    std::cout << this;
}

std::ostream& operator<<(std::ostream& s, const Line& l) {
    s << "{ p1: " << l.p1 << ", p2: " << l.p2 << " }";
    return s;
}

std::unique_ptr<Geometry> Line::divideZ(){
    Point p = *dynamic_cast<Point*>(p1.divideZ().get());
    Point pp = *dynamic_cast<Point*>(p2.divideZ().get());
    Line l(p, pp);
    return std::make_unique<Line>(l);
}

Polygon::Polygon(Point q1, Point q2, Point q3) :
    l1(q1, q2),
    l2(q2, q3),
    l3(q3, q1)
{ }

Polygon::Polygon(Line q1, Line q2, Line q3) :
    l1(q1),
    l2(q2),
    l3(q3)
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

void Polygon::fill(QImage& canvas){
    Line l(l1.p1,l2.p1);
    float dist1 = 1/(l1.p1.pos-l3.p1.pos).length();
    float dist2 = 1/(l2.p1.pos-l3.p1.pos).length();
    float dist = (dist1 > dist2 ? dist1 : dist2) * 0.95;
    float t = 0;
    while(t <= 1){
        //coords transition
        l.p1.pos.x = (1-t)*l1.p1.pos.x + t*l3.p1.pos.x;
        l.p2.pos.x = (1-t)*l2.p1.pos.x + t*l3.p1.pos.x;

        l.p1.pos.y = (1-t)*l1.p1.pos.y + t*l3.p1.pos.y;
        l.p2.pos.y = (1-t)*l2.p1.pos.y + t*l3.p1.pos.y;

        l.p1.pos.z = (1-t)*l1.p1.pos.z + t*l3.p1.pos.z;
        l.p2.pos.z = (1-t)*l2.p1.pos.z + t*l3.p1.pos.z;

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
    return Vector3<float>(l1.p1.pos+l2.p1.pos+l3.p1.pos)/3;
}

void Polygon::print() {
 std::cout << this;
}

std::ostream& operator<<(std::ostream& s, const Polygon& p) {
 s << "{ l1: " << p.l1 << ", l2: " << p.l2 << ", l3: " << p.l3 << " }";
 return s;
}

bool Polygon::operator==(const Polygon& p){
    return (l1 == p.l1 && l2 == p.l2 && l3 == p.l3) || (l1 == p.l2 && l2 == p.l3 && l3 == p.l1) || (l1 == p.l3 && l2 == p.l1 && l3 == p.l2);
}

bool Polygon::operator!=(const Polygon& p){
    return !((*this)==p);
}

std::unique_ptr<Geometry> Polygon::divideZ() {
    Line l(*dynamic_cast<Line*>(l1.divideZ().get()));
    Line ll(*dynamic_cast<Line*>(l2.divideZ().get()));
    Line lll(*dynamic_cast<Line*>(l3.divideZ().get()));
    return std::make_unique<Polygon>(l,ll,lll);
}

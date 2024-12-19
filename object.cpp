#include "object.h"

#include <iostream>
#include <fstream>
#include <sstream>

Object::Object() {}

Object::Object(const std::string FileName) {
    this->loadObj(FileName);
}

Object::Object(std::vector<Polygon> polygons) {
    this->polygons = polygons;
}

void Object::draw(QImage& canvas) {
    for(Polygon &poly : polygons)
        poly.draw(canvas);
}

int Object::loadObj(const std::string FileName) {
    std::string line;
    std::ifstream file;
    std::vector<Vector3<float>> vertices;
    std::vector<Vector3<unsigned int>> faces;

    if(!polygons.empty()) {
        std::cout << "O object ja foi definido anteriormente!" << std::endl;
        return 1;
    }

    file.open("../../objs/"+FileName);
    if (!file.is_open()) {
        std::cout << "Nao foi possivel abrir o arquivo \"" << FileName << '"' << std::endl;
        return 2;
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if(prefix == "v"){
            Vector3<float> v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if(prefix == "f"){
            Vector3<unsigned int> f;
            iss >> f.x >> f.y >> f.z;
            faces.push_back(f);
        }
    }

    for(Vector3<unsigned int> face : faces)
        polygons.push_back(Polygon(
            Point(vertices[face.x-1]),
            Point(vertices[face.y-1]),
            Point(vertices[face.z-1])
        ));

    file.close();
    return 0;
}

std::vector<std::unique_ptr<Geometry>> Object::drawable() {
    std::vector<std::unique_ptr<Geometry>> ptrs;
    for(auto& poly : polygons)
        for(auto& line_ptr : poly.drawable())
            ptrs.push_back(std::move(line_ptr));
    return ptrs;
}

std::unique_ptr<Geometry> Object::multiply(Matrix4x4& matrix) {
    std::vector<Polygon> polygons = this->polygons;
    for(auto& poly : polygons)
        poly = *dynamic_cast<Polygon*>(poly.multiply(matrix).get());
    return std::make_unique<Object>(Object(polygons));
}

Vector3<float> Object::mean() {
    Vector3<float> v(0.0, 0.0, 0.0);
    for(auto& poly : polygons)
        v = v + poly.mean();
    v = v/polygons.size();
    return v;
}

void Object::print() {
    // for(auto& poly : polygons)
        // std::cout << "p1: "<< poly.l1.p1.x << " " << poly.l1.p1.y << " " << poly.l1.p1.z << "p2: "<< poly.l2.p1.x << " " << poly.l2.p1.y << " " << poly.l2.p1.z << "p3: "<< poly.l3.p1.x << " " << poly.l3.p1.y << " " << poly.l3.p1.z << std::endl;
    std::cout << this;
}

std::ostream& operator<<(std::ostream& s, const Object& o) {
    bool isNotFirst=false;
    s << "[ ";
    for(auto& poly : o.polygons) {
        if(isNotFirst)
            s << ", ";
        else
            isNotFirst=true;
        s << poly;
    }
    s << " ]";
    return s;
}

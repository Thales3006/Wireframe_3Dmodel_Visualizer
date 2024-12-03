#ifndef OBJECT_H
#define OBJECT_H

#include <fstream>
#include <sstream>
#include "geometry.h"

class Object : Geometry
{
public:
    std::vector<Polygon> polygons;

    Object();
    Object(const std::string FileName);
    Object(std::vector<Polygon> polygons);

    void draw(QImage& canvas);
    std::vector<std::unique_ptr<Geometry>> drawable();
    std::unique_ptr<Geometry> multiply(Matrix4x4& matrix);
    Vector3<float> mean();

    bool loadObj(const std::string FileName);
};

#endif // OBJECT_H

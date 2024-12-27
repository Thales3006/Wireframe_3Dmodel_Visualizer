#ifndef DISPLAYFILE_H
#define DISPLAYFILE_H

#include "geometry.h"
#include <map>

class DisplayFile
{
public:
    std::map<std::string, std::unique_ptr<Geometry>> objects;

    template <typename T>
    void insert(std::string name, T obj){
        static_assert(std::is_base_of<Geometry, T>::value, "T deve ser derivado de Geometry");
        objects[name] = std::make_unique<T>(obj);
    }

    void insert(std::string name, std::unique_ptr<Geometry> obj);

    void remove(std::string name);

    Vector3<float> getMean(std::string name);

    void drawAll(QImage& canvas, Matrix4x4 cameraMatrix, Vector3<float> viewportSize);
    bool isEmpty();

    void multiply(std::string name, Matrix4x4 mat);
private:
    std::vector<std::unique_ptr<Geometry>> buffer;

};

#endif // DISPLAYFILE_H

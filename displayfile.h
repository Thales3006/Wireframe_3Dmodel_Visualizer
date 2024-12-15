#ifndef DISPLAYFILE_H
#define DISPLAYFILE_H

#include "geometry.h"
#include "object.h"

class DisplayFile
{
public:
    std::vector<std::unique_ptr<Geometry>> displayFileList;
    std::vector<std::string> names;

    void insert(std::string name, Point obj);
    void insert(std::string name, Line obj);
    void insert(std::string name, Polygon obj);
    void insert(std::string name, Object obj);

    void remove(int index);
    void pop();

    std::string getName(int index);
    int getIndex(std::string nome);
    Vector3<float> getMean(std::string nome);
    Matrix4x4 getMatrix(std::string nome);

    void setMatrix(std::string nome, Matrix4x4 matrix);

    void drawAll(QImage& canvas, Matrix4x4 cameraMatrix, Vector3<float> viewportSize);
private:
    std::vector<std::unique_ptr<Geometry>> buffer;

};

#endif // DISPLAYFILE_H

#ifndef DISPLAYFILE_H
#define DISPLAYFILE_H

#include "geometry.h"

class DisplayFile
{
public:
    std::vector<std::unique_ptr<Geometry>> displayFileList;
    std::vector<std::string> names;

    void insert(std::string name, Point obj);
    void insert(std::string name, Line obj);
    void insert(std::string name, Polygon obj);

    void remove(int index);
    void pop();

    std::string getName(int index);

    void drawAll(QImage& canvas);

};

#endif // DISPLAYFILE_H

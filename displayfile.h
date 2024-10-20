#ifndef DISPLAYFILE_H
#define DISPLAYFILE_H

#include "geometry.h"

class DisplayFile
{
public:
    std::vector<std::unique_ptr<Geometry>> displayFileList;

    void insert(Point obj);
    void insert(Line obj);
    void insert(Polygon obj);

    void remove(int index);
    void pop();

    void drawAll(QImage& canvas);

};

#endif // DISPLAYFILE_H

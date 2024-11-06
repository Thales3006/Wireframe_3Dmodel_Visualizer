#ifndef WINDOW_H
#define WINDOW_H

#include "matrix4x4.h"

class Window
{
    Matrix4x4 matrix;
public:
    Vector3<float> pos;
    float angleXY;
    float angleXZ;

    Window(Vector3<float> pos);

    void setPos(Vector3<float> pos);
    void setangleXY(float angle);
    Matrix4x4 getMatrix();
};

#endif // WINDOW_H

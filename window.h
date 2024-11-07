#ifndef WINDOW_H
#define WINDOW_H

#include "matrix4x4.h"

class Window
{
    Matrix4x4 matrix;
private:
    Vector3<float> pos;
    Vector3<float> up;

    void updateMatrix();
public:
    Window(Vector3<float> pos, Vector3<float> up);

    void setPos(Vector3<float> pos);
    void setUp(Vector3<float> up);
    Matrix4x4 getMatrix();
};

#endif // WINDOW_H

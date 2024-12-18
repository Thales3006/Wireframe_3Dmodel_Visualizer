#ifndef WINDOW_H
#define WINDOW_H

#include "matrix4x4.h"

class Window
{
private:
    Matrix4x4 view;
    Matrix4x4 projection;

    Vector3<float> pos;
    Vector3<float> up;
    Vector3<float> right;
    Vector3<float> dir;

    float rightScale;

    void updateView();
public:
    Window(Vector3<float> pos, Vector3<float> up, Vector3<float> dir, float rightScale);

    void setPos(Vector3<float> pos);
    void setUp(Vector3<float> up);
    void setDir(Vector3<float> dir);
    void setRightScale(float scalar);
    void setProjection(Matrix4x4 matrix);

    Vector3<float> getPos();
    Vector3<float> getUp();
    Vector3<float> getDir();
    Vector3<float> getRight();
    float getRightScale();
    Matrix4x4 getView();
};

#endif // WINDOW_H

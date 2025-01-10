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

    float upScale;
    float dirScale;
    float rightScale;
    float FOV;

    float pitch;
    float yaw;
    float roll;

    void updateView();
    void updateVec();
public:
    Window(Vector3<float> pos, Vector3<float> scales, Vector3<float> angles);

    void setPos(Vector3<float> pos);
    void setUp(float scale);
    void setDir(float scale);
    void setAngle(Vector3<float> angle);
    void setRight(float scale);
    void setProjection(bool perspective, float FOV);

    Vector3<float> getPos();
    Vector3<float> getUp();
    Vector3<float> getDir();
    Vector3<float> getRight();
    float getRightScale();
    float getUpScale();
    float getDirScale();
    Matrix4x4 getView();
    float getFOV();
    Vector3<float> getAngle();
};

#endif // WINDOW_H

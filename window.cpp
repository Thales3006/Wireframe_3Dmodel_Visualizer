#include "window.h"

Window::Window(Vector3<float> pos, Vector3<float> up, Vector3<float> dir, float rightScale) {
    this->pos = pos;
    this->up = up.length()? up : Vector3<float>(0.0,1.0,0.0);
    this->dir = dir.length()? dir : Vector3<float>(0.0,0.0,1.0);

    this->rightScale = rightScale? rightScale : 1.0;

    this->right = (up^dir).normalize() * rightScale;

    this->updateMatrix();
}

void Window::setPos(Vector3<float> pos) {
    this->pos = pos;
    this->updateMatrix();
}

#include <QMainWindow>
void Window::setUp(Vector3<float> up) {
    if(up.length()==0)
        return;

    this->up = up;
    this->right = (up^dir).normalize() * rightScale;
    this->updateMatrix();
}

void Window::setDir(Vector3<float> dir){
    if(up.length()==0)
        return;

    this->dir = dir;
    this->right = (up^dir).normalize() * rightScale;
    this->updateMatrix();
}

void Window::setRightScale(float scalar){
    if(up.length()==0)
        return;

    this->rightScale = scalar;
    this->right = (up^dir).normalize() * rightScale;
    this->updateMatrix();
}

Vector3<float> Window::getPos() {
    return pos;
}

Vector3<float> Window::getUp() {
    return up;
}

Vector3<float> Window::getDir() {
    return dir;
}

Vector3<float> Window::getRight() {
    return right;
}

float Window::getRightScale(){
    return rightScale;
}

Matrix4x4 Window::getMatrix() {
    return matrix;
}

void Window::updateMatrix() {

    float roll = up.angle(Vector3<float>(0.0,1.0,0.0));
    float yaw = dir.angle(Vector3<float>(0.0,0.0,1.0));
    this->matrix = Matrix4x4::identity();

    this->matrix.translate(-pos);
    this->matrix.rotate(roll,0,1);
    this->matrix.rotate(yaw,0,2);
    this->matrix.scale(Vector3<float>(1/rightScale, 1/up.length(), 1.0));
}

#include "window.h"

Window::Window(Vector3<float> pos, Vector3<float> up, Vector3<float> dir, float rightScale) {
    this->pos = pos;
    this->up = up.length()? up : Vector3<float>(0.0,1.0,0.0);
    this->dir = dir.length()? dir : Vector3<float>(0.0,0.0,1.0);

    this->rightScale = rightScale? rightScale : 1.0;
    this->right = (up^dir).normalize() * rightScale;

    this->projection = Matrix4x4::identity();

    this->updateView();
}

void Window::setPos(Vector3<float> pos) {
    this->pos = pos;
    this->updateView();
}

#include <QMainWindow>
void Window::setUp(Vector3<float> up) {
    if(up.length()==0)
        return;

    this->up = up;
    this->right = (up^dir).normalize() * rightScale;
    this->updateView();
}

void Window::setDir(Vector3<float> dir){
    if(up.length()==0)
        return;

    this->dir = dir;
    this->right = (up^dir).normalize() * rightScale;
    this->updateView();
}

void Window::setRightScale(float scalar){
    if(up.length()==0)
        return;

    this->rightScale = scalar;
    this->right = (up^dir).normalize() * rightScale;
    this->updateView();
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

Matrix4x4 Window::getView() {
    return view;
}

void Window::updateView() {
    //REFAZER ANGULOS
    float pitch = 0.0;
    float roll = 0.0;
    float yaw = 0.0;
    this->view = Matrix4x4::identity();

    this->view.translate(-pos);

    this->view.rotateY(yaw);
    this->view.rotateZ(pitch);
    this->view.rotateZ(roll);

    this->view.scale(Vector3<float>(1/rightScale, 1/up.length(), 1.0));

    this->view = this->projection * this->view;
}

void Window::setProjection(Matrix4x4 matrix){
    this->projection = matrix;
}

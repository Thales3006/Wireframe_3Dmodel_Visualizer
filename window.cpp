#include "window.h"

Window::Window(Vector3<float> pos, Vector3<float> up) {
    this->pos = pos;
    this->up = up;

    this->updateMatrix();
}

void Window::setPos(Vector3<float> pos) {
    this->pos = pos;
    this->updateMatrix();
}
void Window::setUp(Vector3<float> up) {
    if(up.length()==0)
        return;

    this->up = up;
    this->updateMatrix();
}

Matrix4x4 Window::getMatrix() {
    return matrix;
}

void Window::updateMatrix() {
    float length = up.length();
    Vector3<float> y = Vector3<float>(0.0,1.0,0.0);

    float angle = (up.x > 0? 1.0:-1.0) * ( up * y / length);
    Matrix4x4 newMatrix = Matrix4x4::identity();

    newMatrix.translate(-pos);
    newMatrix.rotate(angle,0,1);
    newMatrix.scale( Vector3<float>(1.0, 1/length, 1.0) );

    this->matrix = newMatrix;
}

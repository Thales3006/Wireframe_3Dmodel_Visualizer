#include "window.h"

Window::Window(Vector3<float> pos) : pos(pos.x,pos.y,pos.z) {
    angleXY=0.0;
    angleXZ=0.0;
}

void Window::setPos(Vector3<float> pos) : pos(pos.x,pos.y,pos.z) {}
void Window::setangleXY(float angle) : angleXY(angle) {

}

Matrix4x4 Window::getMatrix() {
    return matrix;
}

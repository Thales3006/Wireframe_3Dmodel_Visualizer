#include "window.h"

Window::Window(Vector3<float> pos, Vector3<float> scales, Vector3<float> angles) {
    this->pos = pos;

    this->dirScale = scales.x? scales.x : 1.0;
    this->upScale = scales.y? scales.y : 1.0;
    this->rightScale = scales.z? scales.z : 1.0;

    this->projection = Matrix4x4::identity();
    this->FOV = 0;
    this->minDrawDistance = 0.01f;
    this->maxDrawDistance = 1.0;

    this->setAngle(angles);
}

void Window::setPos(Vector3<float> pos) {
    this->pos = pos;
    this->updateView();
}

#include <QMainWindow>
void Window::setUp(float scale) {
    if(scale==0) return;
    upScale = scale;
    updateVec();
}

void Window::setDir(float scale){
    if(scale==0) return;
    dirScale = scale;
    updateVec();
}

void Window::setRight(float scale){
    if(scale==0) return;
    rightScale = scale;
    updateVec();
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

float Window::getDirScale(){
    return dirScale;
}

float Window::getUpScale(){
    return upScale;
}

Matrix4x4 Window::getView() {
    return view;
}

void Window::updateView() {

    this->view = Matrix4x4::identity();

    this->view.translate(-pos);

    this->view.rotateY(-yaw);
    this->view.rotateX(-pitch);
    this->view.rotateZ(-roll);

    this->view.scale(Vector3<float>(1/rightScale, 1/upScale, 1/dirScale));

    this->view = this->projection * this->view;
}

float Window::getMinDrawDistance() {
    return this->minDrawDistance;
}

float Window::getMaxDrawDistance() {
    return this->maxDrawDistance;
}

void Window::setProjection(bool perspective, float FOV, float minDrawDistance, float maxDrawDistance){
    this->FOV = FOV;
    if(minDrawDistance>0 && minDrawDistance<maxDrawDistance) {
        this->minDrawDistance=minDrawDistance;
        this->maxDrawDistance=maxDrawDistance;
    }

    if(!perspective){
        this->projection = Matrix4x4::identity();
        return;
    }
    float s = 1/tan((FOV*M_PI)/360);
    float f = this->maxDrawDistance;
    float n = this->minDrawDistance;

    float a[4][4] = {
        {s, 0.0, 0.0, 0.0},
        {0.0, s, 0.0, 0.0},
        {0.0, 0.0, (f+n)/(f-n), -(2*f*n)/(f-n)},
        {0.0, 0.0, 1.0, 0.0},
     };
    Matrix4x4 mat(a);
    this->projection = mat;
}

float Window::getFOV(){
    return FOV;
}

void Window::setAngle(Vector3<float> angle){
    yaw = angle.x;
    pitch = angle.y;
    roll = angle.z;

    updateVec();
}

Vector3<float> Window::getAngle(){
    return Vector3<float>(yaw, pitch, roll);
}

void Window::updateVec(){
    Matrix4x4 direction = Matrix4x4::identity();

    direction.rotateZ(roll);
    direction.rotateX(pitch);
    direction.rotateY(yaw);

    up = direction * Vector3<float>(0.0, 1.0, 0.0) * upScale;
    dir = direction * Vector3<float>(0.0, 0.0, 1.0) * dirScale;
    right = (up^dir).normalize() * rightScale;
    updateView();
}

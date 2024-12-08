#include "displayfile.h"

void DisplayFile::insert(std::string name, Point obj){
    displayFileList.push_back(std::make_unique<Point>(obj));
    names.push_back(name);
}

void DisplayFile::insert(std::string name, Line obj){
    displayFileList.push_back(std::make_unique<Line>(obj));
    names.push_back(name);
}

void DisplayFile::insert(std::string name, Polygon obj){
    displayFileList.push_back(std::make_unique<Polygon>(obj));
    names.push_back(name);
}
// AAAAAAAAAAAAAAAAAAAAAAA help
void DisplayFile::insert(std::string name, Object obj){
    displayFileList.push_back(std::make_unique<Object>(obj));
    names.push_back(name);
}

void DisplayFile::remove(int index){
    if(displayFileList.size() <= 0 ||index < 0) return;

    displayFileList.erase(displayFileList.begin() + index);
    names.erase(names.begin() + index);
}

void DisplayFile::pop(){
    if(displayFileList.size() <= 0) return;

    displayFileList.pop_back();
    names.pop_back();
}

std::string DisplayFile::getName(int index){
    if((int)displayFileList.size() <= index) return "";

    return names[index];
}

int DisplayFile::getIndex(std::string nome){
    for(int i=0; i < (int)names.size(); i++){
        if(names[i]==nome)
            return i;
    }
    return -1;
}

Vector3<float> DisplayFile::getMean(std::string nome){
    int index = getIndex(nome);
    if(index == -1) return Vector3<float>(0,0,0);

    return displayFileList[index]->mean();
}

Matrix4x4 DisplayFile::getMatrix(std::string nome){
    int index = getIndex(nome);
    if(index == -1) return Matrix4x4();

    return displayFileList[index]->matrix;
}

void DisplayFile::setMatrix(std::string nome, Matrix4x4 matrix){
    int index = getIndex(nome);
    if(index == -1) return;

    displayFileList[index]->matrix = matrix;
}

void DisplayFile::drawAll(QImage& canvas, Matrix4x4 cameraMatrix, Vector3<float> viewportSize){
    Matrix4x4 tempMatrix;
    Matrix4x4 viewportMatrix;
    std::vector<std::unique_ptr<Geometry>> drawableVec;
    buffer.clear();

    viewportSize = viewportSize/2.0;
    viewportMatrix = Matrix4x4::identity();
    viewportMatrix.scale(viewportSize);
    viewportMatrix.translate(viewportSize);

    for(int i = 0; i < (int)displayFileList.size(); i++){
        tempMatrix = cameraMatrix * (displayFileList[i]->matrix);

        drawableVec = displayFileList[i]->multiply(tempMatrix)->drawable();

        if(drawableVec.size()>0)
            for(const auto& drawableObj : drawableVec)
                buffer.push_back( drawableObj->multiply(viewportMatrix) );
    }

    for(const auto& object : buffer)
        object->draw(canvas);
}

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

void DisplayFile::drawAll(QImage& canvas){
    buffer.clear();
    for(int i = 0; i < (int)displayFileList.size(); i++)
        buffer.push_back(displayFileList[i]->multiply(displayFileList[i]->matrix));

    for(const auto& object : buffer)
        object->draw(canvas);
}

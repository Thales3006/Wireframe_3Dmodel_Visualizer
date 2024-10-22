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

void DisplayFile::drawAll(QImage& canvas){
    for(const auto& object : displayFileList){
        object->draw(canvas);
    }
}

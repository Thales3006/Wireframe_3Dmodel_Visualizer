#include "displayfile.h"

void DisplayFile::insert(Point obj){
    displayFileList.push_back(std::make_unique<Point>(obj));
}

void DisplayFile::insert(Line obj){
    displayFileList.push_back(std::make_unique<Line>(obj));
}

void DisplayFile::insert(Polygon obj){
    displayFileList.push_back(std::make_unique<Polygon>(obj));
}

void DisplayFile::remove(int index){
    displayFileList.erase(displayFileList.begin() + index);
}

void DisplayFile::drawAll(QImage& canvas){
    for(int i = 0; i < (int)displayFileList.size(); i++){
        displayFileList[i]->draw(canvas);
    }
}

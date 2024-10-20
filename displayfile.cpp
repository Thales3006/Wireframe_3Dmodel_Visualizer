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
    if(displayFileList.size() <= 0) return;

    displayFileList.erase(displayFileList.begin() + index);

}

void DisplayFile::pop(){
    if(displayFileList.size() <= 0) return;

    displayFileList.pop_back();
}

void DisplayFile::drawAll(QImage& canvas){
    for(const auto& object : displayFileList){
        object->draw(canvas);
    }
}

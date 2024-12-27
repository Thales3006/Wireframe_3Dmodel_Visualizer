#include "displayfile.h"

void DisplayFile::insert(std::string name, std::unique_ptr<Geometry> obj){
    if(!exist(name))
        objects[name] = std::move(obj);
}

void DisplayFile::remove(std::string name){
    if(exist(name))
        objects.erase(name);
}

Vector3<float> DisplayFile::getMean(std::string name){
    if(exist(name))
        return objects[name]->mean();

    return Vector3<float>(0.0,0.0,0.0);
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

    std::map<std::string, std::unique_ptr<Geometry>>::iterator it;
    for(it=objects.begin(); it!=objects.end(); ++it){
        drawableVec = it->second->multiply(cameraMatrix)->drawable();

        for(const auto& drawableObj : drawableVec)
            buffer.push_back( drawableObj->multiply(viewportMatrix) );
    }

    for(const auto& object : buffer)
        object->draw(canvas);
}

bool DisplayFile::empty(){
    return objects.empty();
}

bool DisplayFile::exist(std::string name){
    if (objects.find(name) != objects.end())
        return true;

    return false;
}

void DisplayFile::multiply(std::string name, Matrix4x4 mat){
    if(exist(name))
        objects[name] = objects[name]->multiply(mat);
}

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
    viewportMatrix.scale(Vector3<float>(viewportSize.x,-viewportSize.y,viewportSize.z)/1.1);
    viewportMatrix.translate(viewportSize);

    //THREADING ======== CLIPPING
    auto drawable = [](std::unique_ptr<Geometry>& obj, Matrix4x4& viewportMatrix, Matrix4x4& cameraMatrix, DisplayFile* display){
        std::vector<std::unique_ptr<Geometry>> drawableVec = obj->multiply(cameraMatrix)->drawable();
        display->mtx.lock();
        for(const auto& drawableObj : drawableVec)
            display->buffer.push_back( drawableObj->multiply(viewportMatrix) );
        display->mtx.unlock();
    };

    std::map<std::string, std::unique_ptr<Geometry>>::iterator it;
    for(it=objects.begin(); it!=objects.end(); ++it)
        threads.push_back(std::thread(drawable, std::ref(it->second), std::ref(viewportMatrix), std::ref(cameraMatrix), this) );
    for(auto &thread : threads)
        thread.join();
    threads.clear();

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

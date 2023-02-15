#ifndef OBJECT_H
#define OBJECT_H
#include <eigen3/Eigen/Geometry>
#include <string>

#include "Model.h"
class Object {
   private:
    Eigen::Vector3f scale;

   public:
    Eigen::Vector3f position;
    Eigen::Vector3f rotation;
    Eigen::Vector3f color;
    const char* objFilePath;
    std::shared_ptr<Model> modelLoaded;

    Eigen::Vector3f colorSet[10] = {Eigen::Vector3f(0.f, .4f, 1.f), Eigen::Vector3f(.4f, 1.f, .2f),
                                    Eigen::Vector3f(1.f, .2f, 0.f), Eigen::Vector3f(1.f, 0.f, 1.f), Eigen::Vector3f(1.f, 1.f, 0.f),
                                    Eigen::Vector3f(1.f, .6f, 0.f), Eigen::Vector3f(.4f, .6f, 0.f), Eigen::Vector3f(.2f, .2f, .8f),
                                    Eigen::Vector3f(.4f, 1.f, 1.f), Eigen::Vector3f(1.f, 0.f, .4f)};

    Object();
    ~Object();

    void setScale(Eigen::Vector3f sc);

    Eigen::Vector3f getScale();
};

Object::Object() {
}

Object::~Object() {
}

void Object::setScale(Eigen::Vector3f sc) {
    if (sc[0] <= 0.0) sc[0] = 1.0f;
    if (sc[1] <= 0.0) sc[1] = 1.0f;
    if (sc[2] <= 0.0) sc[2] = 1.0f;
    scale[0] = sc[0];
    scale[1] = sc[1];
    scale[2] = sc[2];
}

Eigen::Vector3f Object::getScale() {
    return scale;
}
#endif
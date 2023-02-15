#ifndef OPTITRACK_H
#define OPTITRACK_H
#include <GL/glut.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
// #include <ctime>
#include <eigen3/Eigen/Geometry>
#include <fstream>
// #include <iostream>
#include <map>
#include <memory>  // unique_ptr and shared_ptr
#include <string>
#include <thread>  // multithreading (using libs=-lpthread)
#include <utility>
#include <vector>

#include "Object.h"
#include "cameras.h"
#include "omp.h"  // parallel for loop (using libs=-lgomp and CXXFLAGS=-fopenmp)

class OptiTrack {
   public:
    // number of rigid bodies
    int& numRigids = cam::numberOfRigids;
    // map data structure consists of all the rigid bodies captured by cameras
    std::map<int, std::shared_ptr<Object>>& rigidObjects = cam::rigidObjectsMap;

    // int coor_accuracy = 6;
    // int move = 0;
    const int numberOfGrids = 20;
    const float gridScale = 10.0f;  // 1.0f = 1 millimiter, 10.0f = 1 centimeters
    const Eigen::Vector3f coordinateTextOffset = {50.f, 100.f, 50.f};

    // changing the object representation mode true = solid, flase = wireframe
    bool isSolidModeOn = true;
    // changing ONLY objects line weight while in the wireframe mode
    const float LINE_WEIGHT = 1.0f;

    OptiTrack();
    ~OptiTrack();

    void run(int argc, char** argv);
    void drawObj(std::shared_ptr<Object>, const Eigen::Vector3f, int);
    void addObjects(std::map<int, std::shared_ptr<Object>>&, const Eigen::Vector3f*);
    void uploadModels(std::vector<const char*> modelPaths);
    void drawText(char*, float, float, float);
    void showCoordinates(Eigen::Vector3f, Eigen::Vector3f, const Eigen::Vector3f, int);
};

OptiTrack::OptiTrack() {
}

OptiTrack::~OptiTrack() {
}

void OptiTrack::run(int argc, char** argv) {
    cam::opti_run(argc, argv);
}

void OptiTrack::uploadModels(std::vector<const char*> modelPaths) {
    for (int i = 1; i <= numRigids; i++) {
        rigidObjects[i]->objFilePath = modelPaths[i - 1];
        std::shared_ptr<Model> model = std::make_shared<Model>();
        model->load(rigidObjects[i]->objFilePath);

        rigidObjects[i]->modelLoaded = model;
    }
}

void OptiTrack::drawText(char* s, float x, float y, float z) {
    // glRasterPos2f(x, y);
    glRasterPos3f(x, y, z);
    // glColor3f(1.0, 1.0, 1.0);
    while (*s) {
        // glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *s);
        s++;
    }
}

void OptiTrack::showCoordinates(Eigen::Vector3f p, Eigen::Vector3f a, const Eigen::Vector3f offset, int id) {
    // display object id
    std::string tempID = "ID: " + std::to_string(id);
    const char* cID = tempID.c_str();
    char* c1ID = strdup(cID);
    drawText(c1ID, offset[0], offset[1], offset[2]);

    float x, y, z;
    x = -1000 * p[0] / (2 * gridScale);
    y = 1000 * p[1] / (2 * gridScale);
    z = -1000 * p[2] / (2 * gridScale);

    // display position coordinates
    // convert string to const char*
    // convert const char* to char*
    std::string tempX = "X: " + std::to_string(x);
    const char* c1 = tempX.c_str();
    char* c1x = strdup(c1);
    drawText(c1x, offset[0], offset[1] - 25, offset[2]);

    std::string tempY = "Y: " + std::to_string(y);
    const char* c2 = tempY.c_str();
    char* c2y = strdup(c2);
    drawText(c2y, offset[0], offset[1] - 50, offset[2]);

    std::string tempZ = "Z: " + std::to_string(z);
    const char* c3 = tempZ.c_str();
    char* c3z = strdup(c3);
    drawText(c3z, offset[0], offset[1] - 75, offset[2]);

    // display rotation angle
    a[0] = 180 - a[0];
    a[1] = 180 - a[1];
    a[2] = 180 - a[2];

    std::string tempRx = "Rx: " + std::to_string(a[0]);
    const char* rx = tempRx.c_str();
    char* rx1 = strdup(rx);
    drawText(rx1, offset[0], offset[1] - 100, offset[2]);

    std::string tempRy = "Ry: " + std::to_string(a[1]);
    const char* ry = tempRy.c_str();
    char* ry1 = strdup(ry);
    drawText(ry1, offset[0], offset[1] - 125, offset[2]);

    std::string tempRz = "Rz: " + std::to_string(a[2]);
    const char* rz = tempRz.c_str();
    char* rz1 = strdup(rz);
    drawText(rz1, offset[0], offset[1] - 150, offset[2]);
}

void OptiTrack::drawObj(std::shared_ptr<Object> obj, const Eigen::Vector3f textOffset, int objID) {
    obj->color = obj->colorSet[objID % 10];
    glColor3f(obj->color[0], obj->color[1], obj->color[2]);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    // translation matrix (the reason that we use minus x and z is the in OpenGL the Z axes is towards the screen and the X axes is to the right, despite the OptiTracktrack)
    glTranslatef(-obj->position[0], obj->position[1], -obj->position[2]);
    // rotation matrices
    glRotatef(obj->rotation[0], 1, 0, 0);
    glRotatef(obj->rotation[1], 0, 1, 0);
    glRotatef(obj->rotation[2], 0, 0, 1);

    glPushMatrix();
    glScalef(100, 100, 100);
    if (!isSolidModeOn) glLineWidth(LINE_WEIGHT);
    // glutSolidCube(100);
    rigidObjects[objID]->modelLoaded->draw();
    glPopMatrix();

    glLineWidth(.5);
    showCoordinates(obj->position, obj->rotation, textOffset, objID);
    glPopMatrix();
}

void OptiTrack::addObjects(std::map<int, std::shared_ptr<Object>>& rigObjsMap, const Eigen::Vector3f* coorTextOffset) {
    // int th_id;
#pragma omp parallel
    {
        for (int i = 1; i <= numRigids; i++) {
            // th_id = omp_get_thread_num();
            // printf("=> thread id: %i\t", th_id);
            drawObj(rigObjsMap[i], *coorTextOffset, i);
        }
    }
}
#endif

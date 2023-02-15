#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#include <GL/glut.h>
#endif

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <eigen3/Eigen/Geometry>
#include <fstream>
#include <map>
#include <memory>  // unique_ptr and shared_ptr
#include <string>
#include <utility>

#include "cameras.h"
///
#include "OptiTrack.h"

///
#include <thread>  // multithreading (using libs=-lpthread)
#include <vector>

#include "Model.h"
#include "Object.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// objects paths
// const char* paths[] = {"Models/mmm.obj", "Models/cone.obj", "Models/cube.obj", "Models/cylinder.obj"};
std::vector<const char*> paths = {"Models/mmm.obj", "Models/cone.obj", "Models/cube.obj", "Models/cylinder.obj"};

std::shared_ptr<OptiTrack> opti = std::make_shared<OptiTrack>();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// OpenGL parameters
int WindowWidth = 500;
int WindowHeight = 500;

// render scene with light
bool isLightOn = true;

// Constants -------------------------------------------------------------------

#define WHEEL_UP 3
#define WHEEL_DOWN 4
#define CAMERA_DISTANCE_MIN 1.0
#define CAMERA_DISTANCE_MAX 100.0

// Global variables ------------------------------------------------------------

static GLint MouseX = 0;
static GLint MouseY = 0;

static double CameraLatitude = 50.0;
static double CameraLongitude = 50.0;
static double CameraDistance = 30.0;

static double EyeX = 150.0;
static double EyeY = 150.0;
static double EyeZ = 150.0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// method signature

// static void Timer(int);
void glut_main(int, char**);
void anim();
void addObjects(std::map<int, std::shared_ptr<Object>>&, const Eigen::Vector3f*);
void draw_axis();
void draw_grid();
void update_camera_location();
void init_scene();
void display();
void resize(int, int);
void mouse(int, int, int, int);
void motion(int, int);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// NatNet parameters
int main(int argc, char** argv) {
    std::thread t1(&OptiTrack::run, opti, argc, argv);
    std::thread t2(glut_main, argc, argv);

    t1.join();
    t2.join();
    return 0;
}

void glut_main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WindowWidth, WindowHeight);
    // glutInitWindowPosition(100, 100);
    glutCreateWindow("OptiTrack App");

    glutDisplayFunc(display);
    glutReshapeFunc(resize);

    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    init_scene();
    // Timer(0);
    glutIdleFunc(anim);

    glutMainLoop();
}

//////////////////////////////////////////////////////////////////////////// start working on integrating glut with the code to see if it shows the spheres at the poit locations
// static void Timer(int value) {
//     move += 0.1;
//     glutPostRedisplay();
//     // 100 milliseconds
//     glutTimerFunc(100, Timer, 0);
// }

void anim() {
    glutPostRedisplay();
}

// void init() {
//     glClearColor(1.0, 1.0, 1.0, 1.0);
// }

void draw_axis() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glLineWidth(4.0);
    glBegin(GL_LINES);
    {
        glColor3f(1.0, 0.0, 0.0);  // X Axis
        glVertex3f(-WindowWidth, 0.0, 0.0);
        glVertex3f(WindowWidth, 0.0, 0.0);

        glColor3f(0.0, 1.0, 0.0);  // Y Axis
        glVertex3f(0.0, -WindowHeight, 0.0);
        glVertex3f(0.0, WindowHeight, 0.0);

        glColor3f(0.0, 0.0, 1.0);  // Z Axis
        glVertex3f(0.0, 0.0, -WindowHeight);
        glVertex3f(0.0, 0.0, WindowHeight);
    }
    glEnd();
    glPopAttrib();
}

void draw_grid() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glPushAttrib(GL_LIGHTING_BIT);
    {
        glDisable(GL_LIGHTING);
        // glColor3f(0.4, 0.4, 0.4);
        glScalef(opti->gridScale * 2, opti->gridScale * 2, opti->gridScale * 2);
        for (int k = 0; k < 2; k++) {
            for (int i = -opti->numberOfGrids; i < opti->numberOfGrids + 1; i++) {
                if (!(i % 10))
                    glColor3f(.8, .8, .8);
                else
                    glColor3f(0.4, 0.4, 0.4);
                glBegin(GL_LINE_STRIP);
                {
                    for (int j = -opti->numberOfGrids; j < opti->numberOfGrids + 1; j++)
                        glVertex3f(k == 0 ? i : j, 0, k == 0 ? j : i);
                }
                glEnd();
            }
        }
    }
    glPopAttrib();
    glPopMatrix();
}

// Update camera ---------------------------------------------------------------

void update_camera_location() {
    double L = CameraDistance * std::cos(M_PI * CameraLongitude / 180.0);
    EyeX = L * -std::sin(M_PI * CameraLatitude / 180.0);
    EyeY = CameraDistance * std::sin(M_PI * CameraLongitude / 180.0);
    EyeZ = L * std::cos(M_PI * CameraLatitude / 180.0);

    glutPostRedisplay();
}

// Initialize scene ------------------------------------------------------------

void init_scene() {
    // glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)WindowWidth / (GLdouble)WindowHeight, 1.0, 750.0);
    glMatrixMode(GL_MODELVIEW);

    if (isLightOn) {
        // adding light
        GLfloat diffuse0[] = {0.8, 0.8, 0.8, 1.0};
        GLfloat ambient0[] = {0.2, 0.2, 0.2, 1.0};
        GLfloat specular0[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat position0[] = {100.0, 100.0, 100.0, 1.0};

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
        glLightfv(GL_LIGHT0, GL_POSITION, position0);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        glEnable(GL_NORMALIZE);
        glEnable(GL_SMOOTH);
        // glPolygonMode(GL_FRONT, GL_FILL);
        // glPolygonMode(GL_BACK, GL_FILL);

        glShadeModel(GL_SMOOTH);
        // glShadeModel(GL_FLAT);
        // glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        if (opti->isSolidModeOn) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_CULL_FACE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }

    //////////////////////////////////////////////////////////////////////////// uploading objects from vector data structure
    opti->uploadModels(paths);

    // update camera location
    update_camera_location();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glLoadIdentity();
    gluLookAt(EyeX, EyeY, EyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    draw_axis();
    draw_grid();

    //////////////////////////////////////////////////////////////////////////// rendering all the objects into the scene
    opti->addObjects(opti->rigidObjects, &opti->coordinateTextOffset);
    ////////////////////////////////////////////////////////////////////////////

    glutSwapBuffers();
}

void resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WindowWidth, WindowWidth, -WindowHeight, WindowHeight, -WindowHeight, WindowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Mouse callback --------------------------------------------------------------

void mouse(int button, int state, int x, int y) {
    MouseX = x;
    MouseY = y;

    switch (button) {
        case WHEEL_UP:
            CameraDistance = (CameraDistance > CAMERA_DISTANCE_MIN ? CameraDistance - 1.0 : CAMERA_DISTANCE_MIN);
            break;
        case WHEEL_DOWN:
            CameraDistance = (CameraDistance < CAMERA_DISTANCE_MAX ? CameraDistance + 1.0 : CAMERA_DISTANCE_MAX);
            break;
    }
    update_camera_location();
}

// Motion callback -------------------------------------------------------------

void motion(int x, int y) {
    CameraLatitude += 180.0 * (double)(x - MouseX) / WindowWidth;
    CameraLongitude += 180.0 * (double)(y - MouseY) / WindowHeight;

    update_camera_location();

    MouseX = x;
    MouseY = y;
}

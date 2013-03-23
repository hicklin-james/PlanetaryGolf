#if defined(__APPLE_CC__)
#include<OpenGL/gl.h>
#include<OpenGL/glu.h>
#include<GLUT/glut.h>
#elif defined(WIN32)
#include<windows.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#else
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<stdint.h>
#endif

#ifndef PLANET_H
#define PLANET_H
#include "imageloader.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include<stdint.h>
#include"CImg.h"
using namespace cimg_library;
using namespace std;

class Planet {
public:
	Planet(int, double, double, double, double, double, double, double);

	void drawPlanet();
	void drawGravityField();
    void moveInWorld();
	void checkX();
	void rotateOuterSphere();

	GLuint loadTexture(Image* image);

	double R;
    
    double xForTranslate;
    double yForTranslate;
    double zForTranslate;
    
    double vx;
    double vy;
    double vz;

	float worldCoords[16];
	double xInWorld;
	double yInWorld;
	double zInWorld;
    
private:
    void checkBoundaries();

	GLuint _textureId;
	GLUquadric *quad;
};



#endif
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

#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H
#include "Planet.h"
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

class SolarSystem {
public:
	void drawSolarSystem();
	std::vector<Planet> planets;
	void addPlanetToSolarSystem( Planet p);
};
#endif
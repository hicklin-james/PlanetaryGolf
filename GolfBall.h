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

#ifndef GOLFBALL_H
#define GOLFBALL_H
#include <iostream>

class GolfBall {
public:
	GolfBall(double, double, double);


	bool fireBall();
	void drawBall(double startX, double startY, double startZ, double xx, double yy, double zz);
	void setVelocity(double initvx, double initvy, double initvz);

	double vx;
	double vy;
	double vz;

	double xPos;
	double yPos;
	double zPos;

	double initXPos;
	double initYPos;
	double initZPos;

	double xForTranslate;
	double yForTranslate;
	double zForTranslate;

    void hardReset();
    
private:

	bool resetIfNeccessary();


};


#endif
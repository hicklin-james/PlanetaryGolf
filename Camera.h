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

#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera {

public:

	Vec3 positionCamera(float eyex, float eyey, float eyez,
						float lookatx, float lookaty, float lookatz,
						float upx, float upy, float upz);

	Vec3 setUpDirectionVector(float eyex, float eyey, float eyez,
							  float lookatx, float lookaty, float lookatz);

};



#endif
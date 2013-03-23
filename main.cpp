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

#include <math.h>			// Header File For Windows Math Library
#include <stdio.h>			// Header File For Standard Input/Output
#include <stdarg.h>			// Header File For Variable Argument Routines
#include <iostream>
#include <stdlib.h>
#include "Planet.h"
#include "SolarSystem.h"
#include <stdio.h>
#include <stdarg.h>
#include "TitleText.h"
#include "StateManager.h"
#include "MenuState.h"

bool invert_pose( float *m );
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/// Global State Variables ///////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

CStateManager* m_pStateManager;

// time increment between calls to idle() in ms,
// currently set to 30 FPS
float dt = 1000.0f*1.0f/30.0f;

// flag to indicate that we should clean up and exit
bool quit = false;

// window handles for mother ship and scout ship
int main_window;


// display width and height
int disp_width=1000, disp_height=1000;

// boolean to control pausing the orbits.
bool isPaused = false;

// Planet numbers stored in a 2d-array
// first column is rotation in degrees
// second column is amount to increment during orbit
// third column is size of planet
float planets[10][3] =
{
	{0,1,0.7},   // Sun
	{0,1.2,0.18},   // Mercury
	{0,1.1,0.25},   // Venus
	{0,1,0.25},   // Earth
	{0,1.7,0.22},   // Mars
	{0,1.3,0.45},   // Jupiter
	{0,1.4,0.23},   // Saturn
	{0,1.3,0.24},   // Uranus
	{0,1.0,0.22},   // Neptune
	{0,0.78,0.13}    // Pluto
};



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/// Initialization/Setup and Teardown ////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

// set up opengl state, allocate objects, etc.  This gets called
// ONCE PER WINDOW, so don't allocate your objects twice!
void init(){
	/////////////////////////////////////////////////////////////
	/// TODO: Put your initialization code here! ////////////////
	/////////////////////////////////////////////////////////////
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	glViewport( 0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT) );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_NORMALIZE );

	// lighting stuff
	GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat diffuse[] = {0.7, 0.7, 0.7, 1.0};
	GLfloat specular[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat position0[] = {1.0, 1.0, 1.0, 0.0};
	glLightfv( GL_LIGHT0, GL_POSITION, position0 );
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	GLfloat position1[] = {-1.0, -1.0, -1.0, 0.0};
	glLightfv( GL_LIGHT1, GL_POSITION, position1 );
	glLightfv( GL_LIGHT1, GL_AMBIENT, ambient );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT1, GL_SPECULAR, specular );

//	GLfloat position2[] = {0.0,1.0,0.0,0.0};
	GLfloat specular2[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat diffuse2[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat ambient2[] = {0.0f, 0.0f, 0.0f, 1.0f};
//	glLightfv(GL_LIGHT2, GL_POSITION, position2 );
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);

	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHT1 );
	glEnable(GL_LIGHT2);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable( GL_COLOR_MATERIAL );

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

}

// free any allocated objects and return
void cleanup(){
	/////////////////////////////////////////////////////////////
	/// TODO: Put your teardown code here! //////////////////////
	/////////////////////////////////////////////////////////////
}



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/// Callback Stubs ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

// window resize callback
void resize_callback( int width, int height ){
	/////////////////////////////////////////////////////////////
	/// TODO: Put your resize code here! ////////////////////////
	/////////////////////////////////////////////////////////////
	disp_width = width;
	disp_height = height;
	glViewport(0,0,width,height);
}

void mouse_callback(int button, int state, int x, int y) {

	m_pStateManager->mouse_callback(button, state, x, y);

}

void mouse_motion_callback(int x, int y) {

	m_pStateManager->mouse_motion_callback(x, y);

}
// keyboard callback
void keyboard_callback( unsigned char key, int x, int y ){
	
	m_pStateManager->keyboard_callback(key, x, y);

	/////////////////////////////////////////////////////////////
	/// TODO: Put your keyboard code here! //////////////////////
	/////////////////////////////////////////////////////////////

}

void keyboard_up_callback(unsigned char key, int x, int y) {

	m_pStateManager->keyboard_up_callback(key, x, y);

}
// display callback
void display_callback( void ){

	// clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pStateManager->display_callback();

	glutSwapBuffers();

}


// not exactly a callback, but sets a timer to call itself
// in an endless loop to update the program
void idle( int value ){

	// if the user wants to quit the program, then exit the
	// function without resetting the timer or triggering
	// a display update
	if( quit ){
		// cleanup any allocated memory
		cleanup();

		// perform hard exit of the program, since glutMainLoop()
		// will never return
		exit(0);
	}

	/////////////////////////////////////////////////////////////
	/// TODO: Put your idle code here! //////////////////////////
	/////////////////////////////////////////////////////////////

	m_pStateManager->idle(0);

	// set the currently active window to the mothership and
	// request a redisplay
	glutSetWindow( main_window );
	glutPostRedisplay();

	// set a timer to call this function again after the
	// required number of milliseconds
	glutTimerFunc( dt, idle, 0 );
}


// inversion routine originally from MESA
bool invert_pose( float *m ){
	float inv[16], det;
	int i;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		m[i] = inv[i] * det;

	return true;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/// Program Entry Point //////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int main( int argc, char **argv ){
	// initialize glut
	m_pStateManager = new CStateManager();
	m_pStateManager->ChangeState(CMenuState::GetInstance(m_pStateManager));

	glutInit( &argc, argv );

	// use double-buffered RGB+Alpha framebuffers with a depth buffer.
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );

	glutInitWindowSize( disp_width, disp_height );
	glutInitWindowPosition( 0, 100 );
	main_window = glutCreateWindow( "Planetary Golf" );
	glutKeyboardFunc( keyboard_callback );
	glutKeyboardUpFunc (keyboard_up_callback);
	glutMouseFunc( mouse_callback );
	glutMotionFunc( mouse_motion_callback );
	glutDisplayFunc( display_callback );
	//glutReshapeFunc( resize_callback );

	glutSetWindow( main_window );
	init();

	// start the idle on a fixed timer callback
	idle( 0 );

	// start the glut main loop
	glutMainLoop();

	return 0;
}


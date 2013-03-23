#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_
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

#include <iostream>
#include <Windows.h>

class CStateManager;

// Base class for the different states
// of the game.
class CGameState
{
public:
	// Constructor
	CGameState(CStateManager* pManager);
	// Destructor
	virtual ~CGameState();

	// The different 'events' functions. Child classes can 
	// implement the ones in which they are interested in.
	virtual void keyboard_callback(unsigned char, int, int ) { }
	virtual void mouse_callback(int, int, int, int )   { }
	virtual void idle(int )  { }
	virtual void display_callback(void )  { }
	virtual void mouse_motion_callback(int, int) { };
	virtual void keyboard_up_callback(unsigned char, int, int) {};
	// Functions called when the state is entered or left
	// (transition from/to another state).
	virtual void EnterState()  { }
	virtual void LeaveState()   { }

	bool CGameState::invert_pose( float *m );

protected:
	// Helper function to switch to a new active state.
	void ChangeState(CGameState* pNewState);

	// The state manager.
	CStateManager* m_pStateManager;
};

#endif  // _GAMESTATE_H_
#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_

#include "GameState.h"

// Manages the different states of the game.
class CStateManager {
public:
	// Default constructor
	CStateManager();
	// Default destructor
	~CStateManager();

	// Switches to another active state.
	void ChangeState(CGameState* pNewState)
	{
		if (m_pActiveState)
			m_pActiveState->LeaveState();
		m_pActiveState = pNewState;
		m_pActiveState->EnterState();
	}

	// Returns the current active state.
	CGameState* GetActiveState()  { 
		return m_pActiveState; 
	}

	// 'Events' function, they are simply redirected to
	// the active state.
	virtual void keyboard_callback(unsigned char key, int x, int y);
	virtual void mouse_callback(int button, int state, int x, int y);
	virtual void idle(int value );
	virtual void display_callback(void);
	virtual void mouse_motion_callback(int x, int y);
	virtual void keyboard_up_callback(unsigned char key, int x, int y);
private:
	// Active State of the game (intro, play, ...)
	CGameState* m_pActiveState;		
};

#endif  // _STATEMANAGER_H_
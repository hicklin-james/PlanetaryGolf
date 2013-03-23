#ifndef _GAMEOVERSTATE_H_
#define _GAMEOVERSTATE_H_

#include "GameState.h"
#include "PlayState.h"

class CGameOverState : public CGameState {
public:
	~CGameOverState();

	void display_callback(void);


	static CGameOverState* GetInstance(CStateManager* pManager);

protected:
	CGameOverState(CStateManager* pManager);

private:
	double FinalScore;
};


#endif
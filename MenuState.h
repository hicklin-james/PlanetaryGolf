#ifndef _MENUSTATE_H_
#define _MENUSTATE_H_

#include "TitleText.h"
#include "GameState.h"
#include "PlayState.h"

class CMenuState : public CGameState {
public:
	~CMenuState();

	void keyboard_callback(unsigned char key, int x, int y);
	void mouse_callback(int button, int state, int x, int y);
	void display_callback(void);
	void idle(int value);
	void EnterState();
	void selectObjects(int x, int y);

	static CMenuState* GetInstance(CStateManager* pManager);

protected:
	CMenuState(CStateManager* pManager);

private:
	TitleText title;
	TitleText newGame;
	TitleText highScores;
	TitleText exitGame;

	int m_iCurrentSelection;

	void DrawButtonBox(float x1, float y1, float x2, float y2);
	void CreateButton(TitleText *titleObj, string name, float x, float y, 
		float scale, bool flashing);
	void DrawButtons();
	void RotatePlanetOnMenu();
};

#endif  // _MENUSTATE_H_
#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

#include "GameState.h"
#include <vector>
#include "Planet.h"
#include "GolfBall.h"
#include "Camera.h"
#include "GameOverState.h"
#include <stdlib.h>

class CPlayState : public CGameState {
public:
	~CPlayState();

	void keyboard_callback(unsigned char key, int x, int y);
	void keyboard_up_callback(unsigned char key, int x, int y);
	void mouse_callback(int button, int state, int x, int y);
	void display_callback(void);
	void idle(int value);
	void mouse_motion_callback(int x, int y);
	void drawPlayArea();
	void drawSpeedBar();
	void DrawHud();
	void CalculateFrameRate();
	void drawWallBorder(double xTrans, double yTrans, double zTrans, 
				        double xScale, double yScale, double zScale);
	static CPlayState* GetInstance(CStateManager* pManager);

	GLuint loadTexture(Image* image);

	int score;
    std::vector<Planet> planets;


protected:
	CPlayState(CStateManager* pManager);

private:
	GLuint _textureId;
	GLUquadric *quad;
};
#endif
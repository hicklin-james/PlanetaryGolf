#include "MenuState.h"
#include "StateManager.h"
#include <iostream>
#include <vector>
#include "Planet.h"

#define BUFSIZE 512
GLuint selectBuf[BUFSIZE];
GLint hits;
GLuint names, *ptr;
bool earthDrawn = false;
std::vector<Planet> planets;

GLint viewPort[14];

CMenuState::CMenuState(CStateManager* pManager): CGameState(pManager) {}

CMenuState::~CMenuState() {}

CMenuState* CMenuState::GetInstance(CStateManager* pManager) {
	static CMenuState Instance(pManager);
	return &Instance;
}

void CMenuState::keyboard_callback(unsigned char key, int x, int y) {

	switch( key ){
	case 27:
		break;
	case 'p':
		std::cout << "hi" << "\n";
		break;
	case 'P':
		break;
	default:
		break;
	}
}

void CMenuState::mouse_callback(int button, int state, int x, int y) {

	//if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP) && (x >= 36) 
	//&& (x <=  244) && (y >= 727) && (y <= 773)) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {

		selectObjects(x, y);
	    unsigned int i, j;

		//printf ("hits = %d\n", hits);
		ptr = (GLuint *) selectBuf;
		for (i = 0; i < hits; i++) { /*  for each hit  */
		names = *ptr;
		printf (" number of names for hit = %d\n", names); ptr++;
		printf("  z1 is %g;", (float) *ptr/0x7fffffff); ptr++;
		printf(" z2 is %g\n", (float) *ptr/0x7fffffff); ptr++;
		printf ("   the name is ");
		for (j = 0; j < names; j++) {     /*  for each name */
			//if (*ptr == 1)
			//	ChangeState(CPlayState::GetInstance(m_pStateManager));
			printf ("%d ", *ptr); ptr++;
		}
		//printf ("\n");
		}
	}
}

void CMenuState::display_callback(void) {
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 70.0f, float(glutGet(GLUT_WINDOW_WIDTH))/float(glutGet(GLUT_WINDOW_HEIGHT)), 0.1f, 2000.0f );
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();  
	glTranslatef(0,0,-11);

	CreateButton(&title, "Planetary Golf", 0, 0, 0.01, true); 

	if (!earthDrawn) {
		Planet earth(0,3,0,0,0,0,0,0);
		planets.push_back(earth);
		earthDrawn = true;
	}
	planets[0].rotateOuterSphere();
	planets[0].drawPlanet();
	//RotatePlanetOnMenu();
	glGetIntegerv(GL_VIEWPORT, viewPort);
	DrawButtons();
	glPopMatrix();
}

void CMenuState::idle(int value) {

}


// TODO for pausing and re-entering game
void CMenuState:: EnterState() {

}

void CMenuState::RotatePlanetOnMenu() {
	glPushMatrix();
	glRotatef(this->title.rot, 0, 1, 0);
	glTranslatef(0,-1.5,0);
	glutSolidSphere(3,10,10);
	glPopMatrix();
}

// Draw buttons in world
void CMenuState::DrawButtons() {

	CreateButton(&newGame, "New Game", -2.5, -11, 0.005, false);
	DrawButtonBox(-7,-6.3,-3,-7.2);

	CreateButton(&highScores, "High Scores", 2.5, -11, 0.005, false);
	DrawButtonBox(-1.85,-6.3, 2.15, -7.2);

	CreateButton(&exitGame, "Exit Game", 7.5, -11, 0.005, false);
	DrawButtonBox(3,-6.3,7,-7.2);
}

// Draws a button at location in world coordinates
void CMenuState::CreateButton(TitleText *titleObj, string name, float x, float y, 
							  float scale, bool flashing) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	titleObj->drawTitle(name, scale, flashing);
	glPopMatrix();
}

// Draws a white box around button locations, using two corner points
void CMenuState::DrawButtonBox(float x1, float y1, float x2, float y2) {

	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glVertex2f(x1,y1);
	glVertex2f(x2, y1);
	glVertex2f(x2,y1);
	glVertex2f(x2,y2);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glVertex2f(x1,y2);
	glVertex2f(x1,y1);
	glEnd();

}

void CMenuState::selectObjects(int x, int y) {

    glSelectBuffer (BUFSIZE, selectBuf);
    (void) glRenderMode (GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
	gluPickMatrix((GLdouble)x, (GLdouble)y, 50, 50, viewPort);
    gluPerspective( 70.0f, float(glutGet(GLUT_WINDOW_WIDTH))/float(glutGet(GLUT_WINDOW_HEIGHT)), 0.1f, 2000.0f );
    glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();  
	glTranslatef(0,0,-11);
	glLoadName(1);
	DrawButtonBox(-7,-6.3,-3,-7.2);
	glPopMatrix();
    glFlush ();

    hits = glRenderMode (GL_RENDER);
}
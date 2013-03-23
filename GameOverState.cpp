#include "GameOverState.h"

CGameOverState::CGameOverState(CStateManager* pManager): CGameState(pManager) {}

CGameOverState::~CGameOverState() {}

CGameOverState* CGameOverState::GetInstance(CStateManager* pManager) {
	static CGameOverState Instance(pManager);
	return &Instance;
}

void CGameOverState::display_callback() {

	  // Temporary disable lighting
    glDisable( GL_LIGHTING );
    
    // Our HUD consists of a simple rectangle
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( -100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f );

    glMatrixMode( GL_MODELVIEW );

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-10,0,0);
    glScalef( 0.05f, 0.05f, 0.05f );

	CPlayState* lastGame = CPlayState::GetInstance(m_pStateManager);
	std::cout << lastGame->score << "\n";

	int index = 0;
	char GameOverText[9] = {'G','A','M','E',' ','O','V','E','R'};
	while( *( GameOverText + index++ ) != '\0' )
               glutStrokeCharacter( GLUT_STROKE_ROMAN, *( GameOverText + index -1 ));


	glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    // Reenable lighting
    glEnable( GL_LIGHTING );
}

#include "PlayState.h"
#include <math.h>
#include "Vector3.h"
#include <vector>

// Camera position
float posx = 0.0, posz = -16.0, posy = 0.0;; // initially 18 units back from origin

float setX = 0.0, setZ = -16.0, setY = 0.0; // Once ball is fired, keep these constant

int timeLeft = 60;

bool updateFPSOnScreen = false;

float deltaMove = 0.0;    // initially camera doesn't move
float framesPerSecond    = 0.0f;       // This will store our fps
float lastTime   = 0.0f;       // This will hold the time from the last frame
double speedCounter = 0;
// Camera direction
float lx = 0.0, lz = 1.0, ly = 0.0; // camera points initially along y-axis

float setLX = 0.0, setLZ = 1.0, setLY = 0.0;

float angleX = 0.0;       // angle of rotation for the camera direction
float angleY = 0.0;
float deltaAngle = 0.0;   // additional angle change when dragging in x
float betaAngle = 0.0;    // additional angle change when dragging in y

// Mouse drag control
int isDragging = 0;       // true when dragging
int xDragStart = 0;       // records the x-coordinate when dragging starts
int yDragStart = 0;       // records y-coordinate when dragging starts

double moveInX = 0;
double moveInZ = 0;
double moveInY = 0;

bool isCameraSaved = false;
bool isSpaceHeldDown = false;

bool flagToResetSpeeds = false;;

// Activate when object is fired:
bool hasBallBeenFired = false;
bool inGravityState = false;

float starInWorld[16];

float currentMatrix[16];
float zStarDire = 0;

Camera camera;

GolfBall ball(0,0,1);

CPlayState::CPlayState(CStateManager* pManager): CGameState(pManager) {}

CPlayState::~CPlayState() {}

CPlayState* CPlayState::GetInstance(CStateManager* pManager) {
	static CPlayState Instance(pManager);
	return &Instance;
}

void CPlayState::keyboard_callback(unsigned char key, int x, int y) {

	switch(key) {
	case 'a':
		moveInZ += lz/128;
		moveInX += lx/128;
		moveInY += ly/128;
		ball.fireBall();
		break;
	case '+':
		break;
	case ' ':
		isSpaceHeldDown = true;
		if (!isCameraSaved) {
			setX = posx;
			setY = posy;
			setZ = posz;
			setLX = lx;
			setLY = ly;
			setLZ = lz;
			isCameraSaved = true;
		}
		break;
	}

}

void CPlayState::keyboard_up_callback(unsigned char key, int x, int y) {
	switch(key ) {
	case ' ':
		flagToResetSpeeds = true;
		isSpaceHeldDown = false;
		hasBallBeenFired = true;
		// Set the initial velocities of the ball
		ball.setVelocity(moveInX, moveInY, moveInZ);
		break;

	}

}
void CPlayState::mouse_callback(int button, int state, int x, int y) {

	if (( button == GLUT_LEFT_BUTTON ) && ( state == GLUT_DOWN )) {

		isDragging = true;
		xDragStart = x;
		yDragStart = y;

	}
	else {

		angleX += deltaAngle;
		angleY += betaAngle;
		isDragging = false;

	}

}

void CPlayState::mouse_motion_callback(int x, int y) {

	if (isDragging == true) {

		deltaAngle = (x - xDragStart) * 0.003;
		betaAngle = (y - yDragStart) * 0.003;
		// camera's direction is set to angle + deltaAngle
		lx = -sin(angleX + deltaAngle);
		lz = cos(angleX + deltaAngle)*cos(angleY+betaAngle);
		ly = -sin(angleY + betaAngle);

	}

}

void CPlayState::display_callback(void) {
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 70.0f, float(glutGet(GLUT_WINDOW_WIDTH))/float(glutGet(GLUT_WINDOW_HEIGHT)), 0.1f, 100.0f );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawSpeedBar();

	// If the ball hasn't been fired, we can still move the camera around
	if (!isCameraSaved) {
		Vec3 Direction = camera.positionCamera(posx,posy,posz,lx,ly,lz,0,1,0);
		ball.drawBall(posx, posy, posz, lx, ly, lz);
		glPopMatrix();
	}
	// otherwise, use the values for the camera that were set when the fire
	// button was pushed.
	else {
		Vec3 Direction = camera.positionCamera(setX,setY,setZ,setLX,setLY,setLZ,0,1,0);
		ball.drawBall(setX, setY, setZ, setLX, setLY, setLZ);
	}


	drawPlayArea();
    glPushMatrix();
	glTranslatef(0,5,0);
	GLfloat position2[] = {0.0,0.0,0.0,0.0};
	glLightfv(GL_LIGHT2, GL_POSITION, position2 );
	glPopMatrix();

    for (int i = 0; i < planets.size(); i++) {
        planets[i].drawPlanet();
        glColor3f(0,0,1);
        planets[i].drawGravityField();
        glColor3f(1,1,1);
    }
    
	//std::cout << "Ball vars: x = " << ball.xPos << " y = " << ball.yPos << " z = " << ball.zPos << "\n";
	//std::cout << "Planet vars: x = " << level1Planet.xInWorld << " y = " << level1Planet.yInWorld << " z = " << level1Planet.zInWorld << "\n" << "\n";
	CalculateFrameRate();

	DrawHud();


}

void CPlayState::idle(int value) {

    // If we have no planets, make a planet at the origin, that isn't moving
    // The first shot should be an easy one!
    if (planets.empty()) {
        Planet planet(0,4,0,0,0,0,0,0);
        planets.push_back(planet);
    }
    
    // Check all planets each time for collisions
    
    
    
    for (int i = 0; i < planets.size(); i++) {
      
        	if (((pow((ball.xPos-planets[i].xInWorld),2)) +
        			(pow((ball.yPos-planets[i].yInWorld),2)) +
        			(pow((ball.zPos-planets[i].zInWorld),2)))
                <= pow((0.1+planets[i].R),2)) {
                std::cout << "The spheres collidided" << "\n";
                
                // trigger two planets to spawn from that location
                // each should have half the size
                
                double size = planets[i].R;
                double startX = planets[i].xInWorld;
                double startY = planets[i].yInWorld;
                double startZ = planets[i].zInWorld;
                planets.erase(planets.begin()+i);
				srand(time(NULL));

				double randoms[6];
				for (int i = 0; i < 3; i++) {
					double a = (((float)rand() / (float)RAND_MAX) * 0.1);
					double b = (((float)rand() / (float)RAND_MAX) * 0.1)+0.1;
					int signA = rand() % 2;
					int signB = rand() %2;

					if (signA == 1)
						randoms[i] = -a;
					else
						randoms[i] = a;

					if (signB == 1)
						randoms[i+3] = -b;
					else
						randoms[i+3] = b;

				}
				Planet planet1(rand() % 5, size/2, startX, startY, startZ, randoms[0],randoms[1],randoms[2]);
                Planet planet2(rand() % 5, size/2, startX, startY, startZ, randoms[3],randoms[4],randoms[5]);
                

                hasBallBeenFired = false;
                ball.hardReset();
                isCameraSaved = false;
                moveInX = 0;
                moveInY = 0;
                moveInZ = 0;
                speedCounter = 0;
                
                planets.push_back(planet1);
                planets.push_back(planet2);
                
                score += 30;
                timeLeft+=30;
                
            }
    
    }
    
    
	if (deltaMove) { // update camera position
		posx += deltaMove * lx * 0.1;
		posz += deltaMove * lz * 0.1;
		posy += deltaMove * ly * 0.1;
	}

	// If the ball has been fired
	if (hasBallBeenFired) {

		// fire the ball, and return false if the ball was reset to its original position
		// This stops the ball from firing again.
		hasBallBeenFired = ball.fireBall();

		// allow camera movement again
		isCameraSaved = false;

		// reset vars -- MOVE TO HELPER
		if (hasBallBeenFired == false) {
			moveInX = 0;
			moveInY = 0;
			moveInZ = 0;
			speedCounter = 0;
		}

	}

	// check for intersects -- MOVE TO HELPER
//	if (((pow((ball.xPos-level1Planet.xInWorld),2)) +
//			(pow((ball.yPos-level1Planet.yInWorld),2)) +
//			(pow((ball.zPos-level1Planet.zInWorld),2)))
//			<= pow((0.1+level1Planet.R),2)) {
//
//				std::cout << "The spheres collided!" << "\n";
//	}

	if (isSpaceHeldDown) {
		moveInX += lx/150;
		moveInY += ly/150;
		moveInZ += lz/150;
		speedCounter += 0.006;
	}

	// If we have no time left in the game, change to game over state
	if (timeLeft <= 0)
		ChangeState(CGameOverState::GetInstance(m_pStateManager));

}

// Draws the play area, the borders. Note that there are walls,
// they are just black!
void CPlayState::drawPlayArea() {

   GLubyte imageData[64][64][3]; // Texture image data
   int value;
   for (int row = 0; row < 64; row++) {
      for (int col = 0; col < 64; col++) {
         // Each cell is 8x8, value is 0 or 255 (black or white)
         value = (((row & 0x8) == 0) ^ ((col & 0x8) == 0)) * 255;
         imageData[row][col][0] = (GLubyte)value;
         imageData[row][col][1] = (GLubyte)value;
         imageData[row][col][2] = (GLubyte)value;
      }
   }
	//glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, 
         GL_UNSIGNED_BYTE, imageData);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	// Floor
	glTexCoord2f (0.0f,0.0f);
	glVertex3f(-30,-30,-18);
	glTexCoord2f (1.0f,0.0f);
	glVertex3f(30,-30,-18);
	glTexCoord2f (1.0f,1.0f);
	glVertex3f(30,-30,30);
	glTexCoord2f (0.0f,1.0f);
	glVertex3f(-30,-30,30);
	// Ceiling
	glColor3f(0.5,0.5,0.5);
	glVertex3f(-30,30,-18);
	glVertex3f(30,30,-18);
	glVertex3f(30,30,30);
	glVertex3f(-30,30,30);
	// Right Wall
	glVertex3f(-30,-30,-18);
	glVertex3f(-30,-30,30);
	glVertex3f(-30,30,30);
	glVertex3f(-30,30,-18);
	// Left Wall
	glVertex3f(30,-30,-18);
	glVertex3f(30,-30,30);
	glVertex3f(30,30,30);
	glVertex3f(30,30,-18);
	// Front Wall
	glVertex3f(30,-30,30);
	glVertex3f(30,30,30);
	glVertex3f(-30,30,30);
	glVertex3f(-30,-30,30);
	// Back Wall
	glVertex3f(30,30,-18);
	glVertex3f(30,-30,-18);
	glVertex3f(-30,-30,-18);
	glVertex3f(-30,30,-18);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// Draw top/back wall line
	drawWallBorder(0,30,-18,60,1,1);
	// Draw bottom/back wall line
	drawWallBorder(0,-30,-18,60,1,1);
	// Draw left/back wall line
	drawWallBorder(30,0,-18,1,60,1);
	// Draw left/top wall line
	drawWallBorder(30,30,6,1,1,48);
	// Draw left/bottom wall line
	drawWallBorder(30,-30,6,1,1,48);
	// Draw right/back wall line
	drawWallBorder(-30,0,-18,1,60,1);
	// Draw right/top wall line
	drawWallBorder(-30,30,6,1,1,48);
	// Draw right/bottom wall line
	drawWallBorder(-30,-30,6,1,1,48);
	// Draw right line on front wall
	drawWallBorder(-30,0,30,1,60,1);
	// Draw left line on front wall
	drawWallBorder(30,0,30,1,60,1);
	// Draw top line on front wall
	drawWallBorder(0,30,30,60,1,1);
	// Draw bottom line on front wall
	drawWallBorder(0,-30,30,60,1,1);

}

// Helper function to draw the wall borders
void CPlayState::drawWallBorder(double xTrans, double yTrans, double zTrans, 
								double xScale, double yScale, double zScale) {

	glColor3f(0,0,1);
	glPushMatrix();
	glTranslatef(xTrans,yTrans,zTrans);
	glScalef(xScale,yScale,zScale);
	glutSolidCube(1);
	glPopMatrix();
	glColor3f(1,1,1);

}
// Function that draws the speed bar and changes its color, depending on how
// long the space bar has been held down for.
void CPlayState::drawSpeedBar() {

	glPushMatrix();
	glTranslatef(1.1,-1.3,-2);
	glPushMatrix();
	glScalef(0.3,0.2,0.1);
	glPopMatrix();

	float red = 0;
	float green = 1;
	double i = 0.4;

	for (i = 0.4; i > 0; i = i - 0.02) {

		glPushMatrix();
		glTranslatef(0,i,0.01);
		glScalef(0.3,0.02,0.1);
		if (speedCounter*0.4 > i)
			glColor4f(red,green,0,1);
		else
			glColor4f(red,green,0,0.5);
		glutSolidCube(1);
		glPopMatrix();

		red += 0.05;
		green -= 0.05; 

	}

	glPopMatrix();
	glPopMatrix();
}

// Function that draws the HUD at the bottom of the screen
void CPlayState::DrawHud()
{
    // Disable lighting
    glDisable( GL_LIGHTING );
    
    // Make the HUD a simple rectangle
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
        glLoadIdentity();
        glOrtho( -100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f );
        glMatrixMode( GL_MODELVIEW );

		// FPS on HUD
        glPushMatrix();
            glLoadIdentity();
            glTranslatef( -90.0f, -90.0f, 0.0f );
            glScalef( 0.05f, 0.05f, 0.05f );

			int fps = (int)framesPerSecond;
			//std::cout << framesPerSecond << "\n";
			char strFPS[7] = {'F','P','S','='};

			char FPS [2];
            sprintf(FPS, "%d", fps);

			strcat(strFPS,FPS);

            int index = 0;
            while( *( strFPS + index++ ) != '\0' )
                glutStrokeCharacter( GLUT_STROKE_ROMAN, *( strFPS + index -1 ));

        glPopMatrix();

		// Time Left on HUD
		glPushMatrix();
        glLoadIdentity();
        glTranslatef( -20.0f, 90.0f, 0.0f );
        glScalef( 0.05f, 0.05f, 0.05f );

		char strTimeLeft[2];
        sprintf(strTimeLeft, "%d", timeLeft);
		char strTime[12] = {'T','I','M','E',' ','L','E','F','T','='};
		strcat(strTime, strTimeLeft);
		index = 0;

		// Change color of time left counter at 30 and 10 seconds
		if (timeLeft > 30)
			glColor3f(0,1,0);
		else if (timeLeft <= 30 && timeLeft > 10)
			glColor3f(0.5,0.5,0);
		else 
			glColor3f(1,0,0);

		while( *( strTime + index++ ) != '\0' )
               glutStrokeCharacter( GLUT_STROKE_ROMAN, *( strTime + index -1 ));
		glPopMatrix();

		// Score on HUD
		// TODO --> when planets are hit, update score
		glPushMatrix();
        glLoadIdentity();
        glTranslatef( -45.0f, -90.0f, 0.0f );
        glScalef( 0.05f, 0.05f, 0.05f );

		char scoreStr[4];
		sprintf(scoreStr, "%d", score);
		char scoreText[10] = {'S','C','O','R','E','='};
		strcat(scoreText, scoreStr);
		index = 0;
		glColor3f(1,1,1);

		while( *( scoreText + index++ ) != '\0' )
               glutStrokeCharacter( GLUT_STROKE_ROMAN, *( scoreText + index -1 ));

		glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    // Re-enable lighting
    glEnable( GL_LIGHTING );
}

// Function to calculate the current frame rate
void CPlayState::CalculateFrameRate()
    {
		// stores our current frames per second
		static float framesPerSecondNow    = 0.0f;
		float currentTime = GetTickCount() * 0.001f;

        ++framesPerSecondNow;
        if( currentTime - lastTime > 1.0f )
        {
			// decrease time by one every second
			timeLeft--;
			// REMOVE LATER
			
			updateFPSOnScreen = true;
            lastTime = currentTime;
            framesPerSecond = framesPerSecondNow;
			framesPerSecondNow = 0;
        }
    }
GLuint CPlayState::loadTexture(Image* image) {

	GLuint textureId;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		image->width, image->height,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		image->pixels);

	return textureId;

}
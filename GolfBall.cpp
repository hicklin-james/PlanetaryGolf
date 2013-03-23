#include "GolfBall.h"

bool hasTexLoaded = false;

// deceleration is constant
double deceleration = 0.004;

// Constructor, default positions x, y and z
GolfBall::GolfBall(double x, double y, double z ) {

	initXPos = x;
	initYPos = y;
	initZPos = z-15;

	xPos = initXPos;
	yPos = initYPos;
	zPos = initZPos;

}

// Responsible for drawing a ball in the world
void GolfBall::drawBall(double x, double y, double z, double lx, double ly, double lz) {

	if (!hasTexLoaded) {
		Image* image = loadBMP("firetexture.bmp");
		_textureId = loadTexture(image);
		delete image;
		hasTexLoaded = true;
	}
	quad = gluNewQuadric();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glPushMatrix();
	// Always take the camera position into account
	xPos = x+lx+xForTranslate;
	yPos = y+ly+yForTranslate;
	zPos = z+lz+zForTranslate;
	glTranslatef(xPos,yPos,zPos);
	//glutSolidSphere(0.1,100,100);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 0.1, 100, 100);
	glColor3f(1,1,1);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);
	//std::cout << "Local ball vars: x = " << posx << " y = " << posy << " z = " << posz << "\n";


}

// Called when the space bar is released - sets vx, vy, and vz
void GolfBall::setVelocity(double initvx, double initvy, double initvz) {
	
	vx = initvx;
	vy = initvy;
	vz = initvz;

}

// Function responsible for moving the ball into the world by vx, vy, and vz.
// Also decreases vy by deceleration on each iteration, so the ball drops 
// downwards. Returns a boolean: true if no reset was necessary (so the 
// ball continues moving), false otherwse(so the ball has reset to the 
// default position
bool GolfBall::fireBall() {

	xForTranslate += vx;
	yForTranslate += vy;
	zForTranslate += vz;

	vy -= deceleration;
	
	return !resetIfNeccessary();
	//std::cout << deceleration << "\n";
}


// Reset the ball position if it's current position is at the floor, the back wall,
// or either of the sides. Returns true if a reset was neccessary, false otherwise
//
bool GolfBall::resetIfNeccessary() {

	if (zPos > 30 || yPos < -30 || xPos > 30 || xPos < -30) {
		xPos = initXPos;
		yPos = initYPos;
		zPos = initZPos;
		xForTranslate = 0;
		yForTranslate = 0;
		zForTranslate = 0;
		vx = 0;
		vy = 0;
		vz = 0;
		return true;
	}

	return false;

}

void GolfBall::hardReset() {
    
    xPos = initXPos;
    yPos = initYPos;
    zPos = initZPos;
    xForTranslate = 0;
    yForTranslate = 0;
    zForTranslate = 0;
    vx = 0;
    vy = 0;
    vz = 0;
}

GLuint GolfBall::loadTexture(Image* image) {

	GLuint textureId;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		image->width, image->height,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		image->pixels);

	return textureId;

}
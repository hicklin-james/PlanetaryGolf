#include "GolfBall.h"

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

	glPushMatrix();
	// Always take the camera position into account
	xPos = x+lx+xForTranslate;
	yPos = y+ly+yForTranslate;
	zPos = z+lz+zForTranslate;
	glTranslatef(xPos,yPos,zPos);
	glColor3f(0,1,0);
	glutSolidSphere(0.1,100,100);
	glColor3f(1,1,1);
	glPopMatrix();

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
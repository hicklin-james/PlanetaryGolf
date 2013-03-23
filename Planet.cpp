#include"Planet.h"

double xRot = 0.0;
bool hasImageLoaded = false;
float rotate;

const char *bitMapStrings[5] = {"earth.bmp", "moon.bmp","mars.bmp",
	"mercury.bmp","jupiter.bmp"};

// Draws the planet
void Planet::drawPlanet() {

	quad = gluNewQuadric();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPushMatrix();
	checkBoundaries();
    moveInWorld();
	glTranslatef(xInWorld, yInWorld, zInWorld);
	glRotatef(xRot*0.8,0,1,0);
	glRotatef(-90,1,0,0);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, R, 100, 100);
	//glutSolidSphere(R,100,100);
	glColor3f(1,1,1);

	glDisable(GL_TEXTURE_2D);

	//std::cout << xForIntersectTest << " " << yForIntersectTest << " " << zForIntersectTest << "\n";

	glPopMatrix();
}

void Planet::drawGravityField() {
	glPushMatrix();
	rotateOuterSphere();
	glTranslatef(xInWorld, yInWorld, zInWorld);
	glRotatef(xRot,0,1,0);
	glRotatef(90,1,0,0);
	glutWireSphere(R*2, 10, 10);
	glPopMatrix();
}

Planet::Planet (int bitMap, double radius, double xStart, double yStart, double zStart, double vX, double vY, double vZ) {
	R = radius;
	xInWorld = xStart;
	yInWorld = yStart;
	zInWorld = zStart;
	vx = vX;
    vy = vY;
    vz = vZ;

	const char *planetBitMap = bitMapStrings[bitMap];
	Image* image = loadBMP(planetBitMap);
	_textureId = loadTexture(image);
	delete image;
}

void Planet::moveInWorld() {
    
    
    xInWorld += vx;
    yInWorld += vy;
    zInWorld += vz;
    
    
}

void Planet::checkBoundaries() {
    
    if (xInWorld+R*2 > 30 || xInWorld-R*2 < -30)
        vx = -vx;
    
    if (yInWorld+R*2 > 30 || yInWorld-R*2 < -30)
        vy = -vy;
    
    if (zInWorld+R*2 > 30 || zInWorld-R*2 < -17)
        vz = -vz;
    
}

void Planet::rotateOuterSphere() {
	xRot += 1;
}

GLuint Planet::loadTexture(Image* image) {

	GLuint textureId;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		image->width, image->height,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		image->pixels);

	return textureId;

}


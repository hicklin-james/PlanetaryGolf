#include "TitleText.h"

void TitleText::drawTitle(string s, float x, bool col) {

	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR,currentColor);
	GLfloat len = s.length() / 2;
	glPushMatrix();
	glTranslatef(-4.3,4,0);
	glScalef(x,x,x);
	for (int i = 0; i < s.length(); i++) {
		if (col == true)
			glColor3f(1.0f*float(cos(rot/20.0f)),1.0f*float(sin(rot/25.0f)),1.0f-0.5f*float(cos(rot/17.0f)));
		glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)s.at(i));
		glTranslatef(1,0,0);
	}
	glPopMatrix();
	rot += 1;
	glColor4f(currentColor[0], currentColor[1], currentColor[2],
		currentColor[3]);
}
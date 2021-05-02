/*
*	rendering contains functions to render UI and game objects
*
*	Main Contributer: Dean
*/

#include "rendering.h"

// Menu
int menuItemDistance = 80;
int menuItemCount = 3;
char* menuItems[3] = { "Play", "Options", "Exit" };
float menuSelectorAngle = 0;

// Menu Rendering
void renderMenu(int select) {
	glPushMatrix();

	// Draw menu items
	for (int i = 0; i < menuItemCount; ++i) {
		drawText(screenWidth / 8, screenHeight / 2 - i * menuItemDistance, menuItems[i], 6, 0.3);
	}

	// Draw selector
	glPushMatrix();
	glTranslatef(screenWidth / 8 - menuItemDistance / 2, screenHeight / 2 + (select + 0.2) * menuItemDistance, 0.0f);
	glRotatef(menuSelectorAngle, 0.0f, 0.0f, 1.0f);
	drawPentagon(15);
	glPopMatrix();

	if (menuSelectorAngle < 360) {
		menuSelectorAngle += 0.1;
	}
	else {
		menuSelectorAngle = 0;
	}

	glPopMatrix();
}

// Object Rendering
void renderAsteroids() {
	// Create the asteroid in OpenGl
	for (int i = 0; i < getAsteroidCount(); ++i) {
		float x = getAsteroid(i)->position.x;
		float y = getAsteroid(i)->position.y;
		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((getAsteroid(i)->angle * 180.0 / PI) - 90, 0.0, 0.0, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawAsteroid(getAsteroid(i)->vertices, getAsteroid(i)->vertexCount, getAsteroid(i)->size);

		glPopMatrix();
	}
}

void renderPlayer() {
	if (getPlayer() != NULL) {
		float x = getPlayer()->position.x;
		float y = getPlayer()->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((vectorAngle(getPlayer()->direction) * 180.0 / PI) - 90, 0.0, 0.0, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawPlayer();

		glPopMatrix();
	}
}

void renderBullets() {
	for (int i = 0; i < getBulletCount(); ++i) {
		float x = getBullet(i)->position.x;
		float y = getBullet(i)->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawBullet();

		glPopMatrix();
	}
}

void renderEngineParticles() {
	for (int i = 0; i < getParticleCount(); ++i) {
		float x = getParticle(i)->position.x;
		float y = getParticle(i)->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((getParticle(i)->angle * 180.0 / PI) - 90, 0, 0, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawPentagon(getParticle(i)->size);

		glPopMatrix();
	}
}

void renderBlackHoles() {
	for (int i = 0; i < getBlackHoleCount(); ++i) {
		glPushMatrix();
		glTranslatef(getBlackHole(i)->position.x, getBlackHole(i)->position.y, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawBlackHole(getBlackHole(i));

		glPopMatrix();
	}
}

// Object Rendering
void drawAsteroid(PositionVector* vertices, int vertexCount, int size) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < vertexCount; ++i) {
		glVertex2f(vertices[i].x * size, vertices[i].y * size);
	}
	glEnd();
}

void drawPlayer() {
	glBegin(GL_TRIANGLES);
	glVertex2f(-20, -20);
	glVertex2f(0, 20);
	glVertex2f(0, -10);
	glVertex2f(0, 20);
	glVertex2f(20, -20);
	glVertex2f(0, -10);
	glEnd();
}

void drawBullet() {
	drawPentagon(3);
}

void drawBlackHole(BlackHole* bh) {
	drawCircle(bh->radius);
}

// Rendering Shapes
void drawPentagon(float radius) {
	glBegin(GL_LINE_LOOP);
	glVertex2f(0, 1 * radius);
	glVertex2f(0.9511 * radius, 0.309 * radius);
	glVertex2f(0.5878 * radius, -0.809 * radius);
	glVertex2f(-0.5878 * radius, -0.809 * radius);
	glVertex2f(-0.9511 * radius, 0.309 * radius);
	glEnd();
}

void drawCircle(float radius) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; ++i) {
		glVertex2f(cos(i * PI / 180) * radius, sin(i * PI / 180) * radius);
	}
	glEnd();
}

// Rendering Other
void drawText(float x, float y, const char* text, int length, float size) {
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glScalef(size, size, 1.0);

	for (int i = 0; i < length; ++i) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}
	glPopMatrix();
}
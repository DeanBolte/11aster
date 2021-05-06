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

// Rendering Calls
void renderSplash() {
	glPushMatrix();

	drawText(screenWidth / 2 - 1.5 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'A'), screenHeight / 1.4, "11Aster", 7, 0.5);
	drawText(screenWidth / 2 - 7 / 5 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'P'), screenHeight / 8, "by Dean Bolte", 13, 0.2);

	glPopMatrix();
}

void renderInGame() {
	// InGame Global Render
	glPushMatrix();

	// create offset for rendering the screen offset from the centre
	PositionVector renderOffset;
	renderOffset = multiplyVector(getPlayer()->moveVector, -0.1f);

	renderUI();

	glTranslatef(renderOffset.x, renderOffset.y, 0.0f);

	// Render Order
	renderAsteroids();
	renderBlackHoles();
	renderBullets();
	renderEngineParticles();
	renderPlayer();

	glPopMatrix();
}

void renderGameOver() {
	glPushMatrix();

	drawText(screenWidth / 2 - 4 / 2 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'G'), screenHeight / 1.4, "Game Over.", 10, 0.5);
	drawText(screenWidth / 2 - 2.6 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'P'), screenHeight / 8, "Press any key to play again!", 28, 0.2);

	glPopMatrix();
}

void renderMenu(int select) {
	glPushMatrix();

	float x = screenWidth / 8;
	float y = screenHeight / 2;

	// Draw menu items
	for (int i = 0; i < menuItemCount; ++i) {
		drawText(x, y - i * menuItemDistance, menuItems[i], 6, 0.3);
	}

	// Draw selector
	glPushMatrix();
	glTranslatef(x - menuItemDistance / 2, y + (0.2 - select) * menuItemDistance, 0.0f);
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

void renderPause(int select) {
	glPushMatrix();

	float x = screenWidth / 8 + (getPlayer()->position.x - screenWidth / 2);
	float y = screenHeight / 2 + (getPlayer()->position.y - screenHeight / 2);

	// Draw menu items
	for (int i = 0; i < menuItemCount; ++i) {
		drawText(x, y - i * menuItemDistance, menuItems[i], 6, 0.3);
	}

	// Draw selector
	glPushMatrix();
	glTranslatef(x - menuItemDistance / 2, y + (0.2 - select) * menuItemDistance, 0.0f);
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

void renderUI() {
	glPushMatrix();

	// Player UI
	Player* player = getPlayer();

	float x = 24 + (player->position.x - screenWidth / 2);
	float y = screenHeight - 24 + (player->position.y - screenHeight / 2);

	// Player health container
	glPushMatrix();

	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y - 18);
	glVertex2f(x, y);
	glVertex2f(x + player->maxHp * 24, y);
	glVertex2f(x + player->maxHp * 24, y - 18);
	glEnd();

	glPopMatrix();

	// Player healthometer
	glPushMatrix();

	glBegin(GL_POLYGON);
	glVertex2f(x, y - 18);
	glVertex2f(x, y);
	glVertex2f(x + player->hp * 24, y);
	glVertex2f(x + player->hp * 24, y - 18);
	glEnd();

	glPopMatrix();

	// Player speed
	float speed = vectorLength(player->moveVector) / 10;
	float maxSpeed = player->maxVelocity / 10;

	// Player speed container
	glPushMatrix();

	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y - 36);
	glVertex2f(x, y - 24);
	glVertex2f(x + maxSpeed, y - 24);
	glVertex2f(x + maxSpeed, y - 36);
	glEnd();

	glPopMatrix();

	// Player speedometer
	glPushMatrix();

	glBegin(GL_POLYGON);
	glVertex2f(x, y - 36);
	glVertex2f(x, y - 24);
	glVertex2f(x + speed, y - 24);
	glVertex2f(x + speed, y - 36);
	glEnd();

	glPopMatrix();

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
	glBegin(GL_LINE_LOOP);
	glVertex2f(-20, -20);
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

void drawHeart(float size) {
	// semicircle offset values
	float xOffset = size * cos(PI / 4) / 2;
	float yOffset = size * sin(PI / 4) / 2;
	
	// First semicircle
	glPushMatrix();
	glTranslatef(-xOffset, yOffset, 0.0f);
	glRotatef(45, 0.0f, 0.0f, 1.0f);
	drawSemiCircle(size / 2);
	glPopMatrix();

	// Second semicircle
	glPushMatrix();
	glTranslatef(xOffset, yOffset, 0.0f);
	glRotatef(-45, 0.0f, 0.0f, 1.0f);
	drawSemiCircle(size / 2);
	glPopMatrix();

	// Bottom of heart
	glPushMatrix();
	glBegin(GL_POLYGON);
	glVertex2f(-2 * xOffset, 0);
	glVertex2f(0, -2 * yOffset);
	glVertex2f(2 * xOffset, 0);
	glVertex2f(0, 2 * yOffset);
	glEnd();
	glPopMatrix();
}

void drawSemiCircle(float radius) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < 180; ++i) {
		glVertex2f(cos(i * PI / 180) * radius, sin(i * PI / 180) * radius);
	}
	glEnd();
}

void drawContainer(float x, float y, float height, float length, float fill) {
	// Draw container
	glPushMatrix();

	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x, y + height);
	glVertex2f(x + length, y + height);
	glVertex2f(x + length, y);
	glEnd();

	glPopMatrix();

	// Draw filling
	glPushMatrix();

	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x, y + height);
	glVertex2f(x + fill, y + height);
	glVertex2f(x + fill, y);
	glEnd();

	glPopMatrix();
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
/*
*	rendering contains functions to render UI and game objects
*
*	Main Contributer: Dean
*/

#include "rendering.h"

// Menu
int menuItemDistance = 80;
int menuItemCount = 3;
const char* menuItems[3] = { "Play", "Options", "Exit" };
float menuSelectorAngle = 0;

// Constructors
Renderer::Renderer(float screen_width, float screen_height) {
	// Init colour data
	highColour = new Colour(0.004f, 0.922f, 0.373f);
	lowColour = new Colour(0.145f, 0.204f, 0.184f);
	warnColour = new Colour(0.8f, 0.1f, 0.1f);

	// set screen dimensions
	float screenWidth;
	float screenHeight;
}

// Rendering Calls
void Renderer::renderSplash() {
	glPushMatrix();

	drawText(screenWidth / 2 - 1.5 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'A'), screenHeight / 1.4, "11Aster", 7, 0.5);
	drawText(screenWidth / 2 - 7 / 5 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'P'), screenHeight / 8, "by Dean Bolte", 13, 0.2);

	glPopMatrix();
}

void Renderer::renderInGame() {
	// InGame Global Render
	glPushMatrix();

	// create offset for rendering the screen offset from the centre
	PositionVector renderOffset;
	renderOffset = multiplyVector(getPlayer()->getMoveVector(), -0.1f);

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

void Renderer::renderGameOver() {
	glPushMatrix();

	drawText(screenWidth / 2 - 4 / 2 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'G'), screenHeight / 1.4, "Game Over.", 10, 0.5);
	drawText(screenWidth / 2 - 2.6 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'P'), screenHeight / 8, "Press any key to play again!", 28, 0.2);

	glPopMatrix();
}

void Renderer::renderMenu(int select) {
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

void Renderer::renderPause(int select) {
	glPushMatrix();

	float x = screenWidth / 8 + (getPlayer()->getPosition().x - screenWidth / 2);
	float y = screenHeight / 2 + (getPlayer()->getPosition().y - screenHeight / 2);

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

void Renderer::renderUI() {
	// Player UI
	Player* player = getPlayer();

	// UI position
	float x = 36 + (player->getPosition().x - screenWidth / 2);
	float y = screenHeight - 36 + (player->getPosition().y - screenHeight / 2);

	// Player health icon
	glPushMatrix();
	glTranslatef(x - 20, y + 8, 0.0f);
	drawHeart(10);
	glPopMatrix();

	// Player health container
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	drawContainer(16, player->getMaxHp() * 24, player->getHp() * 24);
	glPopMatrix();

	// Player speed
	float speed = vectorLength(player->getMoveVector()) / 10;
	float maxSpeed = player->getMaxVelocity() / 10;

	// Player speed icon
	glPushMatrix();
	glTranslatef(x - 24, y - 26, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(8, 8);
	glVertex2f(8, 8);
	glVertex2f(0, 16);
	glVertex2f(8, 0);
	glVertex2f(16, 8);
	glVertex2f(16, 8);
	glVertex2f(8, 16);
	glEnd();
	glPopMatrix();

	// Player speed container
	glPushMatrix();
	glTranslatef(x, y - 24, 0.0f);
	drawContainer(12, maxSpeed, speed);
	glPopMatrix();
}

// Object Rendering
void Renderer::renderAsteroids() {
	// Create the asteroid in OpenGl
	for (int i = 0; i < getAsteroidCount(); ++i) {
		float x = getAsteroid(i)->position.x;
		float y = getAsteroid(i)->position.y;
		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((getAsteroid(i)->angle * 180.0 / PI) - 90, 0.0, 0.0, 1.0);

		glColor3f(highColour->getRed(), highColour->getGreen(), highColour->getBlue());

		drawAsteroid(getAsteroid(i)->vertices, getAsteroid(i)->vertexCount, getAsteroid(i)->size);

		glPopMatrix();
	}
}

void Renderer::renderPlayer() {
	if (getPlayer() != NULL) {
		float x = getPlayer()->getPosition().x;
		float y = getPlayer()->getPosition().y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((vectorAngle(getPlayer()->getDirection()) * 180.0 / PI) - 90, 0.0, 0.0, 1.0);

		glColor3f(highColour->getRed(), highColour->getGreen(), highColour->getBlue());

		drawPlayer();

		glPopMatrix();
	}
}

void Renderer::renderBullets() {
	for (int i = 0; i < getBulletCount(); ++i) {
		float x = getBullet(i)->position.x;
		float y = getBullet(i)->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);

		glColor3f(highColour->getRed(), highColour->getGreen(), highColour->getBlue());

		drawBullet();

		glPopMatrix();
	}
}

void Renderer::renderEngineParticles() {
	for (int i = 0; i < getParticleCount(); ++i) {
		float x = getParticle(i)->position.x;
		float y = getParticle(i)->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((getParticle(i)->angle * 180.0 / PI) - 90, 0, 0, 1.0);

		glColor3f(highColour->getRed(), highColour->getGreen(), highColour->getBlue());

		drawPentagon(getParticle(i)->size);

		glPopMatrix();
	}
}

void Renderer::renderBlackHoles() {
	for (int i = 0; i < getBlackHoleCount(); ++i) {
		glPushMatrix();
		glTranslatef(getBlackHole(i)->position.x, getBlackHole(i)->position.y, 1.0);

		glColor3f(highColour->getRed(), highColour->getGreen(), highColour->getBlue());

		drawBlackHole(getBlackHole(i));

		glPopMatrix();
	}
}

// Object Rendering
void Renderer::drawAsteroid(PositionVector* vertices, int vertexCount, int size) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < vertexCount; ++i) {
		glVertex2f(vertices[i].x * size, vertices[i].y * size);
	}
	glEnd();
}

void Renderer::drawPlayer() {
	glBegin(GL_LINE_LOOP);
	glVertex2f(-20, -20);
	glVertex2f(0, 20);
	glVertex2f(20, -20);
	glVertex2f(0, -10);
	glEnd();
}

void Renderer::drawBullet() {
	drawPentagon(3);
}

void Renderer::drawBlackHole(BlackHole* bh) {
	drawCircle(bh->radius);
}

// Rendering Shapes
void Renderer::drawPentagon(float radius) {
	glBegin(GL_LINE_LOOP);
	glVertex2f(0, 1 * radius);
	glVertex2f(0.9511 * radius, 0.309 * radius);
	glVertex2f(0.5878 * radius, -0.809 * radius);
	glVertex2f(-0.5878 * radius, -0.809 * radius);
	glVertex2f(-0.9511 * radius, 0.309 * radius);
	glEnd();
}

void Renderer::drawCircle(float radius) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; ++i) {
		glVertex2f(cos(i * PI / 180) * radius, sin(i * PI / 180) * radius);
	}
	glEnd();
}

void Renderer::drawHeart(float size) {
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

void Renderer::drawSemiCircle(float radius) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < 180; ++i) {
		glVertex2f(cos(i * PI / 180) * radius, sin(i * PI / 180) * radius);
	}
	glEnd();
}

void Renderer::drawContainer(float height, float length, float fill) {
	// Draw container
	glPushMatrix();

	glBegin(GL_LINE_LOOP);
	glVertex2f(0, 0);
	glVertex2f(0, height);
	glVertex2f(length, height);
	glVertex2f(length, 0);
	glEnd();

	glPopMatrix();

	// Draw filling
	glPushMatrix();

	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(0, height);
	glVertex2f(fill, height);
	glVertex2f(fill, 0);
	glEnd();

	glPopMatrix();
}

// Rendering Other
void Renderer::drawText(float x, float y, const char* text, int length, float size) {
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glScalef(size, size, 1.0);

	for (int i = 0; i < length; ++i) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}
	glPopMatrix();
}
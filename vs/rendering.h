/*
*	rendering contains functions to render UI and game objects
*
*	Main Contributer: Dean
*/

#include "factory.h"

// Menu
#define PLAY 0
#define OPTIONS 1
#define EXIT 2

// View walls
#define LEFT_WALL 1
#define RIGHT_WALL 2
#define BOTTOM_WALL 3
#define TOP_WALL 4

class Renderer {
private:
	float screenWidth;
	float screenHeight;

	// Colours
	Colour* highColour;
	Colour* lowColour;
	Colour* warnColour;

	// Screen functions
	int boolOutOfBounds(PositionVector position, float offset);

	// Object Rendering
	void renderAsteroids();
	void renderPlayer();
	void renderBullets();
	void renderEngineParticles();

	// Object Rendering
	void drawAsteroid(Asteroid* asteroid);
	void drawPlayer();
	void drawBullet();

	// Rendering Shapes
	void drawPentagon(float radius);
	void drawCircle(float radius);
	void drawHeart(float size);
	void drawSemiCircle(float radius);
	void drawContainer(float height, float length, float fill);

	// Rendering Other
	void drawText(float x, float y, const char* text, int length, float size);

public:
	// Constructors
	Renderer(float screen_width, float screen_height);

	// Rendering Calls
	void renderSplash();
	void renderInGame();
	void renderGameOver();
	void renderMenu(int select);
	void renderPause(int select);
	void renderUI();

};
/*
*	rendering contains functions to render UI and game objects
*
*	Main Contributer: Dean
*/

#include "objects.h"

// Menu
#define PLAY 0
#define OPTIONS 1
#define EXIT 2

float screenWidth;
float screenHeight;

// Rendering Calls
void renderSplash();
void renderInGame();
void renderGameOver();
void renderMenu(int select);
void renderPause(int select);
void renderUI();

// Object Rendering
void renderAsteroids();
void renderPlayer();
void renderBullets();
void renderEngineParticles();
void renderBlackHoles();

// Object Rendering
void drawAsteroid(PositionVector* vertices, int vertexCount, int size);
void drawPlayer();
void drawBullet();
void drawBlackHole(BlackHole* bh);

// Rendering Shapes
void drawPentagon(float radius);
void drawCircle(float radius);
void drawHeart(float size);
void drawSemiCircle(float radius);

// Rendering Other
void drawText(float x, float y, const char* text, int length, float size);
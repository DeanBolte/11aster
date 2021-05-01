/*
*	rendering contains functions to render UI and game objects
*
*	Main Contributer: Dean
*/

#include "objects.h"

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

// Rendering Other
void drawText(float x, float y, const char* text, int length, float size);
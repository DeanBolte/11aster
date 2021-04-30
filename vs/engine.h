/*
*	engine contains the main functionality for gameplay
*
*	Main Author: Dean
*/
#pragma once

#include "elements.h"
 
#define MAX_ASTEROIDS 500
#define MAX_BULLETS 100
#define MAX_ENGINE_PARTICLES 1000
#define MAX_BLACKHOLES 3

#define PLAYER_PARTICLE_SIZE 5

#define ASTEROID_SPAWN_ACCELERATION_MULTIPLIER 0.01
#define ASTEROID_SPAWN_RADIUS_MULTIPLIER 1.1
#define ASTEROID_SPLIT_RADIUS_MULTIPLIER 5
#define ASTEROID_SPLIT_ANGLE PI / 6
#define ASTEROID_PUFF_COUNT 50
#define ASTEROID_PUFF_SIZE 20

#define MIN_BH_DISTANCE_FROM_PLAYER 50

// Arena walls
#define LEFT_WALL 1
#define RIGHT_WALL 2
#define BOTTOM_WALL 3
#define TOP_WALL 4

// Game States
#define SPLASH 0
#define INITIALISING 1
#define IN_GAME 2
#define GAME_OVER 3

// Colours
RGB highColour;
RGB lowColour;
RGB warnColour;

// Colours
void initColours();

//		Game Engine Calls
// Initialising Calls
void init(int screen_width, int screen_height);
void initRound();
void initKeys();

// Update Calls
void update(float delta, float Width, float Height);
void updateGame(float delta);

// Render Calls
void render();
void renderSplash();
void renderInGame();
void renderGameOver();

// Input Calls
void inputKeyboard(const char* key, int pressed);
void inputMouse(const char* input, int pressed);
void inputGameStart(const char* input, int pressed);
void inputInGame(const char* input, int pressed);
void inputGameOver(const char* input, int pressed);

//		Object creation
void createAsteroid(float x, float y);
void createPlayer(float x, float y);
void createParticle(PositionVector position, PositionVector velocity, int maxSize);
void createBlackHole(PositionVector position);

//		Gameplay
void gameOver();

//		Player Actions
void fireCannonPlayer(float delta);

//		Collision Detection
void checkCollisions();
void asteroidCollisions();
void blackHoleCollision();
int collidingWithBlackHole(PositionVector position, float radius);

//		Asteroid Actions
void explodeAsteroid(Asteroid* asteroid, int index);
void splitAsteroid(Asteroid* asteroid);

//		BlackHole Actions
void updateBlackHole(float delta, BlackHole* bh);

//		Movement
// Movement calls
PositionVector movePosition(float delta, PositionVector position, PositionVector movement);
int boolOutOfBounds(PositionVector position, float offset);

// Asteroid Movement
void moveAsteroid(float delta, Asteroid* asteroid);
void removeAsteroid(Asteroid* asteroid, int index);

// Player Movement
void movePlayer(float delta);
void rotatePlayer(float delta);
void acceleratePlayer(float delta, int dir);

// Bullet Movement
void moveBullets(float delta);
void cullBullet(Bullet* bullet, int index);
void removeBullet(Bullet* bullet, int index);

// Particle Movement
void moveParticles(float delta);
void cullParticle(Particle* particle, int index);

// Black Hole
void applyGravity(float delta);
PositionVector getAccelerationVector(float delta, BlackHole* bh, PositionVector objectPosition);

// Object Rendering
void renderAsteroids();
void renderPlayer();
void renderBullets();
void renderEngineParticles();
void renderBlackHoles();
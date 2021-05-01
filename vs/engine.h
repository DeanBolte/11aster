/*
*	engine contains the main functionality for gameplay
*
*	Main Author: Dean
*/
#pragma once

#include "physics.h"

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

//		Gameplay
void gameOver();

//		Player Actions
void fireCannonPlayer(float delta);

//		BlackHole Actions
void updateBlackHole(float delta, BlackHole* bh);

// Screen Check
int boolOutOfBounds(PositionVector position, float offset);

// Object Removal
void removeAsteroid(Asteroid* asteroid, int index);
void removeBullet(Bullet* bullet, int index);
void cullParticle(Particle* particle, int index);
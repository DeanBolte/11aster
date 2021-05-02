/*
*	engine contains the main functionality for gameplay
*
*	Main Author: Dean
*/
#pragma once

#include "physics.h"
#include "rendering.h"

// View walls
#define LEFT_WALL 1
#define RIGHT_WALL 2
#define BOTTOM_WALL 3
#define TOP_WALL 4

// Game States
#define SPLASH 0
#define INITIALISING 1
#define MENU 2
#define IN_GAME 3
#define PAUSED 4
#define GAME_OVER 5

// Menu
int menuSelect;

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
void updatePlayer(float delta, Player* player);

// Render Calls
void render();
void renderSplash();
void renderInGame();
void renderGameOver();

// Input Calls
void inputKeyboard(const char* key, int pressed);
void inputMouse(const char* input, int pressed);
void inputGameStart(const char* input, int pressed);
void inputControls(const char* input, int pressed);
void inputGameOver(const char* input, int pressed);

//		Gameplay
void gameOver();

//		Player Actions
void fireCannonPlayer(float delta);

//		BlackHole Actions
void updateBlackHole(float delta, BlackHole* bh);

// Screen Check
int boolOutOfBounds(PositionVector position, float offset);

void cullBullet(Bullet* bullet, int index);
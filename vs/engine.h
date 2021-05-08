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
#define OPTIONS 3
#define IN_GAME 4
#define PAUSED 5
#define GAME_OVER 6

// Menu
#define MENU_SELECT_COOLDOWN 0.08f

class Engine {
private:
	// Game Data
	int gameState;

	// This ensures that the game over screen only accepts an input after a new key is pressed during the game over screen
	int inputDuringGameOver;

	// Pause menu variable
	int inputDuringPause;

	// Keyboard Inputs
	int key_right;
	int key_left;
	int key_down;
	int key_up;
	int key_space;
	int key_firing;
	int key_select;
	int key_pause;

	// Menu
	int menuSelect;
	float menuSelectCoolDown;

	float screenWidth;
	float screenHeight;

	Renderer* renderer;
	
	// Initialising functions
	void initRound();
	void resetKeys();

	// World creation
	void generateAsteroidBelt(float x, float y, int distance, int range, int amount);

	// Update functions
	void updateMenu(float delta);
	void updatePause(float delta);
	void updateSelect(float delta);
	void selectMenuItem(int select);
	void selectPauseItem(int select);
	void updateGame(float delta);

	// Input functions
	void inputGameStart(const char* input, int pressed);
	void inputControls(const char* input, int pressed);
	void inputGameOver(const char* input, int pressed);

	// Game updates
	void gameOver();

	// Player updates
	void updatePlayer(float delta, Player* player);
	void fireCannonPlayer(float delta);

	// Screen functions
	int boolOutOfBounds(PositionVector position, float offset);

	// Misc
	void cullBullet(Bullet* bullet, int index);

public:
	// Constructors
	Engine(int screen_width, int screen_height);
	
	// Game Engine Calls
	void update(float delta, float Width, float Height);
	void render();

	// Input Calls
	void inputKeyboard(const char* key, int pressed);
	void inputMouse(const char* input, int pressed);

};
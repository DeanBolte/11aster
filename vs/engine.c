/*
*	engine contains the main functionality for gameplay
*
*	Main Author: Dean
*/
#include "engine.h"

// Game Data
int gameState = SPLASH;

// This ensures that the game over screen only accepts an input after a new key is pressed during the game over screen
int inputDuringGameOver = 0;

// Pause menu variable
int inputDuringPause = 0;

// Keyboard Inputs
int key_right = 0;
int key_left = 0;
int key_down = 0;
int key_up = 0;
int key_space = 0;
int key_firing = 0;
int key_select = 0;
int key_pause = 0;

// Menu
int menuSelect = 0;
float menuSelectCoolDown = 0;

// Colours
void initColours() {
	// Init colour data
	highColour.r = 0.004f;
	highColour.g = 0.922f;
	highColour.b = 0.373f;

	lowColour.r = 0.145f;
	lowColour.g = 0.204f;
	lowColour.b = 0.184f;

	warnColour.r = 0.8f;
	warnColour.g = 0.1f;
	warnColour.b = 0.1f;
}

// Game Engine Calls
void init(int screen_width, int screen_height) {
	// Initialise Colours
	initColours();

	// Update screen size
	screenWidth = screen_width;
	screenHeight = screen_height;
}

void initRound() {
	// Return input values to initial values
	resetKeys();
	
	// Create Player at starting position
	createPlayer(screenWidth / 2, screenHeight / 2);

	// Create some asteroids in a ring around the player
	generateAsteroidBelt(getPlayer()->position.x, getPlayer()->position.y, 1250, 1000, 100);

	// Start Game
	gameState = IN_GAME;
}

void resetKeys() {
	// Keyboard Inputs
	key_right = 0;
	key_left = 0;
	key_down = 0;
	key_up = 0;
	key_space = 0;
	key_firing = 0;
	key_select = 0;
	key_pause = 0;
}

// World Creation
void generateAsteroidBelt(float x, float y, int distance, int range, int amount) {
	for (int i = 0; i < amount; ++i) {
		float ran = rand() % 100;
		float angle = ran / 100 * (2 * PI);
		float length = distance + rand() % range - range / 2;
		PositionVector position = multiplyVector(angleToUnitVector(angle), length);
		createAsteroid(position.x + x, position.y + y);
	}
}

void update(float delta, float Width, float Height) {
	// Update screen size
	screenWidth = Width;
	screenHeight = Height;

	// gameplay switch
	switch (gameState) {
	case SPLASH:
		// could add some asteroids or a black hole here to make it seem cool, but that is out of scope
		break;
	case MENU:
		updateMenu(delta);
		break;
	case INITIALISING:
		initRound();
		break;
	case IN_GAME:
		updateGame(delta);
		break;
	case PAUSED:
		updatePause(delta);
		break;
	case GAME_OVER:
		// I had an idea here for making the game over text wobble, just a cool addition that i dont have the time to implement
		break;
	default:
		break;
	}
}

void updateMenu(float delta) {
	updateSelect(delta);

	// Menu select
	if (key_select == 1) {
		selectMenuItem(menuSelect);
	}
}

void updatePause(float delta) {
	updateSelect(delta);

	// Menu select
	if (key_select == 1) {
		selectPauseItem(menuSelect);
		resetKeys();
	}
}

void updateSelect(float delta) {
	// Menu movement
	int dir = key_down - key_up;
	if (dir != 0) {
		if (menuSelectCoolDown > 0) {
			menuSelectCoolDown -= delta;
		}
		else {
			menuSelect += dir;
			if (menuSelect > 2) {
				menuSelect -= 3;
			}
			else if (menuSelect < 0) {
				menuSelect += 3;
			}

			menuSelectCoolDown = MENU_SELECT_COOLDOWN;
		}
	}
}

void selectMenuItem(int select) {
	switch (select) {
	case PLAY:
		gameState = INITIALISING;
		break;
	case OPTIONS:
		break;
	case EXIT:
		exit(EXIT_SUCCESS);
		break;
	default:
		break;
	}
}

void selectPauseItem(int select) {
	switch (select) {
	case PLAY:
		gameState = IN_GAME;
		break;
	case OPTIONS:
		break;
	case EXIT:
		exit(EXIT_SUCCESS);
		break;
	default:
		break;
	}
}

void updateGame(float delta) {
	// Physics Call - Update Movement
	physicsMovements(delta);

	// Update player data
	updatePlayer(delta, getPlayer());

	// Update BlackHoles
	for(int i = 0; i < getBlackHoleCount(); ++i) {
		updateBlackHole(delta, getBlackHole(i));
	}

	// Check Collisions
	physicsCollisions();

	// Reset select and enter pause
	if (key_pause == 1) {
		menuSelect = 0;
		gameState = PAUSED;
	}
}

// Main Render Call
void render() {
	glPushMatrix();
	// Screen effects
	

	// Render Switch
	switch (gameState) {
	case SPLASH:
		renderSplash();
		break;
	case MENU:
		renderMenu(menuSelect);
		break;
	case IN_GAME:
		renderInGame();
		break;
	case PAUSED:
		renderInGame();
		renderPause(menuSelect);
		break;
	case GAME_OVER:
		renderGameOver();
		break;
	default:
		break;
	}

	glPopMatrix();
}

// Input Calls
void inputKeyboard(const char* key, int pressed) {
	// Mouse Input Switch
	switch (gameState) {
	case SPLASH:
		inputGameStart(key, pressed);
		break;
	case MENU:
	case IN_GAME:
	case PAUSED:
		inputControls(key, pressed);
		break;
	case GAME_OVER:
		inputGameOver(key, pressed);
		break;
	default:
		break;
	}
}

void inputMouse(const char* input, int pressed) {
	// Mouse Input Switch
	switch (gameState) {
	case SPLASH:
		inputGameStart(input, pressed);
		break;
	case MENU:
	case IN_GAME:
	case PAUSED:
		inputControls(input, pressed);
		break;
	case GAME_OVER:
		inputGameOver(input, pressed);
		break;
	default:
		break;
	}
}

void inputGameStart(const char* input, int pressed) {
	if (pressed == 0) {
		gameState = MENU;
	}
}

void inputControls(const char* input, int pressed) {
	if (input == "left") {
		key_left = pressed;
	}
	if (input == "right") {
		key_right = pressed;
	}
	if (input == "up") {
		key_up = pressed;
	}
	if (input == "down") {
		key_down = pressed;
	}
	if (input == "space") {
		key_space = pressed;
	}
	if (input == "shoot") {
		key_firing = pressed;
		key_select = pressed;
	}
	if (input == "test") {
		gameOver();
		gameState = INITIALISING;
	}
	if (input == "pause") {
		key_pause = pressed;
	}
}

void inputGameOver(const char* input, int pressed) {
	if (pressed == 0 && inputDuringGameOver == 1) {
		gameState = INITIALISING;
	}
	else {
		inputDuringGameOver = 1;
	}
}

// Gameplay
void gameOver() {
	// Free all game objects to reset the game
	freeAllObjects();

	// Reset game over key
	inputDuringGameOver = 0;

	gameState = GAME_OVER;
}

// Player action
// Frame update for player data
void updatePlayer(float delta, Player* player) {
	// Update players particle cooldown
	if (player->particleCoolDown <= 0) {
		player->particleCoolDown = PLAYER_PARTICLE_INTERVAL;
	}
	else {
		player->particleCoolDown -= delta;
	}

	// Player Actions
	acceleratePlayer(delta, key_up, key_space);
	rotatePlayer(delta, key_left - key_right);

	fireCannonPlayer(delta);

	// Check player status
	if (player->hp <= 0) {
		gameOver();
	}
}

void fireCannonPlayer(float delta) {
	// Create bullet
	Player* player = getPlayer();
	if (player->cannonCoolDown <= 0 && getBulletCount() < MAX_BULLETS && key_firing == 1) {
		// Reset cooldown
		player->cannonCoolDown = PLAYER_CANNON_COOLDOWN;
		
		// Create bullet
		createBullet(player->position, player->direction);
	}

	// reduce cooldown for players cannon
	if (player->cannonCoolDown > 0) {
		player->cannonCoolDown -= delta;
	}
}

// BlackHole Actions
void updateBlackHole(float delta, BlackHole* bh) {
	// update radius
	bh->radius += bh->pulseDirection * bh->radiusOutPulse * delta;
	if (bh->radius > bh->radiusOutPulse) {
		bh->pulseDirection = -1;
	}
	else if (bh->radius < bh->radiusInPulse) {
		bh->pulseDirection = 1;
	}
}

// could extend this to provide an array of ints and then use that to represent more than one side
// however i deem this out of scope
int boolOutOfBounds(PositionVector position, float offset) {
	// Return > 0 if position is out of bounds
	// value represents side
	// 0 = left, 1 = right, 2 = bottom, 3 = top
	int outOfBounds = 0;
	if (position.x < 0 - offset) {
		outOfBounds = LEFT_WALL;
	}
	else if (position.x > screenWidth + offset) {
		outOfBounds = RIGHT_WALL;
	}
	else if (position.y < 0 - offset) {
		outOfBounds = BOTTOM_WALL;
	}
	else if (position.y > screenHeight + offset) {
		outOfBounds = TOP_WALL;
	}
	return outOfBounds;
}

// misc
void cullBullet(Bullet* bullet, int index) {
	if (boolOutOfBounds(bullet->position, 32) > 0) {
		freeBullet(index);
	}
}
/*
*	engine contains the main functionality for gameplay
*
*	Main Author: Dean
*/
#include "engine.h"
#include "rendering.h"

// Game Data
float screenWidth = 0.0;
float screenHeight = 0.0;
int gameState = SPLASH;

// This ensures that the game over screen only accepts an input after a new key is pressed during the game over screen
int inputDuringGameOver = 0;

// Keyboard Inputs
int key_right = 0;
int key_left = 0;
int key_down = 0;
int key_up = 0;
int key_space = 0;

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
	// Create Player at starting position
	createPlayer(256, 256);

	// Return input values to initial values
	initKeys();

	// Start Game
	gameState = IN_GAME;
}

void initKeys() {
	// Keyboard Inputs
	key_right = 0;
	key_left = 0;
	key_down = 0;
	key_up = 0;
	key_space = 0;
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
	case INITIALISING:
		initRound();
		break;
	case IN_GAME:
		updateGame(delta);
		break;
	case GAME_OVER:
		// I had an idea here for making the game over text wobble, just a cool addition that i dont have the time to implement
		break;
	default:
		break;
	}
}

void updateGame(float delta) {
	// Physics Call - Update Movement
	physicsMovements(delta);

	// Get playerdata
	Player* player = getPlayer();

	// Update players particle cooldown
	if (player->particleCoolDown <= 0) {
		player->particleCoolDown = PLAYER_PARTICLE_INTERVAL;
	}
	else {
		player->particleCoolDown -= delta;
	}

	// Update BlackHoles
	for(int i = 0; i < getBlackHoleCount(); ++i) {
		updateBlackHole(delta, getBlackHole(i));
	}

	// Player Actions
	acceleratePlayer(delta, key_up);
	rotatePlayer(delta, key_left - key_right);

	fireCannonPlayer(delta);

	// Check Collisions
	physicsCollisions();
}

void render() {
	// Render Switch
	switch (gameState) {
	case SPLASH:
		renderSplash();
		break;
	case INITIALISING:
		// Nothing should ever really need to be rendered during intialisation
		// However i felt the need to include it here in case its needed later
		// perhaps a loading screen to ensure that users with low end pcs know something is happening
		break;
	case IN_GAME:
		renderInGame();
		break;
	case GAME_OVER:
		renderGameOver();
		break;
	default:
		break;
	}
}

void renderSplash() {
	glPushMatrix();

	drawText(screenWidth / 2 - 1.5 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'A'), screenHeight / 1.4, "11Aster", 7, 0.5);
	drawText(screenWidth / 2 - 14 / 5 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'P'), screenHeight / 8, "Press any key to play!", 24, 0.2);

	glPopMatrix();
}

void renderInGame() {
	// InGame Global Render
	glPushMatrix();

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
	drawText(screenWidth / 2 - 14 / 5 * glutStrokeWidth(GLUT_STROKE_ROMAN, 'P'), screenHeight / 8, "Press any key to play again!", 28, 0.2);
	
	glPopMatrix();
}

void inputKeyboard(const char* key, int pressed) {
	// Mouse Input Switch
	switch (gameState) {
	case SPLASH:
		inputGameStart(key, pressed);
		break;
	case INITIALISING:
		// This case is not really needed
		// could be used for an interactive load screen
		break;
	case IN_GAME:
		inputInGame(key, pressed);
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
	case INITIALISING:
		// This case is not really needed
		// could be used for an interactive load screen
		break;
	case IN_GAME:
		inputInGame(input, pressed);
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
		gameState = INITIALISING;
	}
}

void inputInGame(const char* input, int pressed) {
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
		getPlayer()->firing = pressed;
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

// Player action
void fireCannonPlayer(float delta) {
	// Create bullet
	Player* player = getPlayer();
	if (player->cannonCoolDown <= 0 && getBulletCount() < MAX_BULLETS && player->firing == 1) {
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

void cullBullet(Bullet* bullet, int index) {
	if (boolOutOfBounds(bullet->position, 32) > 0) {
		freeBullet(index);
	}
}
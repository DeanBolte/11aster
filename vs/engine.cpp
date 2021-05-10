/*
*	engine contains the main functionality for gameplay
*
*	Main Author: Dean
*/
#include "engine.h"

// Constructors
Engine::Engine(int screen_width, int screen_height) {
	// Set heights
	screenWidth = screen_width;
	screenHeight = screen_height;

	// Game Data
	gameState = SPLASH;

	// This ensures that the game over screen only accepts an input after a new key is pressed during the game over screen
	inputDuringGameOver = 0;

	// Pause menu variable
	inputDuringPause = 0;

	// Keyboard Inputs
	key_right = 0;
	key_left = 0;
	key_down = 0;
	key_up = 0;
	key_space = 0;
	key_firing = 0;
	key_select = 0;
	key_pause = 0;

	// Menu
	menuSelect = 0;
	menuSelectCoolDown = 0;

	// Rendering engine
	renderer = new Renderer(screen_width, screen_height);
}

void Engine::initRound() {
	// Return input values to initial values
	resetKeys();
	
	// Create Player at starting position
	createPlayer(screenWidth / 2, screenHeight / 2);

	// Create some asteroids in a ring around the player
	generateAsteroidBelt(getPlayer()->getPosition().x, getPlayer()->getPosition().y, 1250, 1000, 100);

	// Start Game
	gameState = IN_GAME;
}

void Engine::resetKeys() {
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
void Engine::generateAsteroidBelt(float x, float y, int distance, int range, int amount) {
	for (int i = 0; i < amount; ++i) {
		float ran = rand() % 100;
		float angle = ran / 100 * (2 * PI);
		float length = distance + rand() % range - range / 2;
		PositionVector position = multiplyVector(angleToUnitVector(angle), length);
		createAsteroid(position.x + x, position.y + y);
	}
}

void Engine::update(float delta, float Width, float Height) {
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

void Engine::updateMenu(float delta) {
	updateSelect(delta);

	// Menu select
	if (key_select == 1) {
		selectMenuItem(menuSelect);
	}
}

void Engine::updatePause(float delta) {
	updateSelect(delta);

	// Menu select
	if (key_select == 1) {
		selectPauseItem(menuSelect);
		resetKeys();
	}
}

void Engine::updateSelect(float delta) {
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

void Engine::selectMenuItem(int select) {
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

void Engine::selectPauseItem(int select) {
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

void Engine::updateGame(float delta) {
	// Physics Call - Update Movement
	physicsMovements(delta);

	// Update player data
	updatePlayer(delta, getPlayer());

	// Check Collisions
	physicsCollisions();

	// Reset select and enter pause
	if (key_pause == 1) {
		menuSelect = 0;
		gameState = PAUSED;
	}
}

// Main Render Call
void Engine::render() {
	glPushMatrix();
	// Screen effects
	

	// Render Switch
	switch (gameState) {
	case SPLASH:
		renderer->renderSplash();
		break;
	case MENU:
		renderer->renderMenu(menuSelect);
		break;
	case IN_GAME:
		renderer->renderInGame();
		break;
	case PAUSED:
		renderer->renderInGame();
		renderer->renderPause(menuSelect);
		break;
	case GAME_OVER:
		renderer->renderGameOver();
		break;
	default:
		break;
	}

	glPopMatrix();
}

// Input Calls
void Engine::inputKeyboard(const char* key, int pressed) {
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

void Engine::inputMouse(const char* input, int pressed) {
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

void Engine::inputGameStart(const char* input, int pressed) {
	if (pressed == 0) {
		gameState = MENU;
	}
}

void Engine::inputControls(const char* input, int pressed) {
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
		//getPlayer()->getHp() -= 1;
	}
	if (input == "pause") {
		key_pause = pressed;
	}
}

void Engine::inputGameOver(const char* input, int pressed) {
	if (pressed == 0 && inputDuringGameOver == 1) {
		gameState = INITIALISING;
	}
	else {
		inputDuringGameOver = 1;
	}
}

// Gameplay
void Engine::gameOver() {
	// Free all game objects to reset the game
	freeAllObjects();

	// Reset game over key
	inputDuringGameOver = 0;

	gameState = GAME_OVER;
}

// Player action
// Frame update for player data
void Engine::updatePlayer(float delta, Player* player) {
	// Player Actions
	player->acceleratePlayer(delta, key_up);
	int dir = key_left - key_right;
	player->rotatePlayer(delta * dir * DEGREE_OF_ROTATION);

	// Space brakes
	if (key_space == 1) {
		player->brakePlayer(delta);
	}

	fireCannonPlayer(delta);

	// Update players particle cooldown
	if (player->getParticleCoolDown() <= 0 && key_up == 1) {
		player->incrementParticleCoolDown(PLAYER_PARTICLE_INTERVAL);

		createParticle(player->getPosition(), player->getDirection());
	}
	player->incrementParticleCoolDown(-delta);

	// Check player status
	if (player->getHp() <= 0) {
		gameOver();
	}
}

void Engine::fireCannonPlayer(float delta) {
	// Create bullet
	Player* player = getPlayer();
	if (player->getCannonCoolDown() <= 0 && getBulletCount() < MAX_BULLETS && key_firing == 1) {
		// Reset cooldown
		player->incrementCannonCoolDown(PLAYER_CANNON_COOLDOWN);
		
		// Create bullet
		createBullet(player->getPosition(), player->getDirection());
	}

	// reduce cooldown for players cannon
	if (player->getCannonCoolDown() > 0) {
		player->incrementCannonCoolDown(-delta);
	}
}

// could extend this to provide an array of ints and then use that to represent more than one side
// however i deem this out of scope
int Engine::boolOutOfBounds(PositionVector position, float offset) {
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
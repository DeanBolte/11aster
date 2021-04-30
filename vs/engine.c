/*
*	engine contains the main functionality for gameplay
*
*	Main Author: Dean
*/
#include "engine.h"

// Game Data
float screenWidth = 0.0;
float screenHeight = 0.0;
int gameState = SPLASH;

// This ensures that the game over screen only accepts an input after a new key is pressed during the game over screen
int inputDuringGameOver = 0;

// Ship variables and constants
double DEGREE_OF_ROTATION = 2 * PI;

// Keyboard Inputs
int key_right = 0;
int key_left = 0;
int key_down = 0;
int key_up = 0;
int key_space = 0;

// Object Data
Asteroid* asteroidArray[MAX_ASTEROIDS];
int asteroidCount = 0;

Bullet* bulletArray[MAX_BULLETS];
int bulletCount = 0;

Player* playerData = NULL;

Particle* particleArray[MAX_ENGINE_PARTICLES];
int particleCount = 0;

BlackHole* blackHoleArray[MAX_BLACKHOLES];
int blackHoleCount = 0;

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
	// Asteroids
	for (int i = 0; i < asteroidCount; ++i) {
		moveAsteroid(delta, asteroidArray[i]);
	}

	// Player Actions
	acceleratePlayer(delta, key_up);
	rotatePlayer(delta);
	movePlayer(delta);
	fireCannonPlayer(delta);

	// Update players particle cooldown
	if (playerData->particleCoolDown <= 0) {
		playerData->particleCoolDown = PLAYER_PARTICLE_INTERVAL;
	}
	else {
		playerData->particleCoolDown -= delta;
	}

	// Update BlackHoles
	for(int i = 0; i < blackHoleCount; ++i) {
		updateBlackHole(delta, blackHoleArray[i]);
	}

	// Apply gravity from black holes
	// Applies to Asteroids, Bullets, particles and the Player
	applyGravity(delta);

	// Bullet Movement
	moveBullets(delta);

	// Particle Movement
	moveParticles(delta);

	// Check Collisions
	checkCollisions();
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
		playerData->firing = pressed;
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

// Object Creation
void createAsteroid(float x, float y) {
	if (asteroidCount < MAX_ASTEROIDS) {
		Asteroid* asteroid = initAsteroid(x, y);
		asteroidArray[asteroidCount] = asteroid;
		++asteroidCount;
	}
}

void createPlayer(float x, float y) {
	if (playerData != NULL) {
		destructPlayer(playerData);
	}
	playerData = initPlayer(x, y);
}

void createParticle(PositionVector position, PositionVector velocity, int maxSize) {
	if (particleCount < MAX_ENGINE_PARTICLES) {
		Particle* particle = initParticle(position, velocity, maxSize);
		particleArray[particleCount] = particle;
		++particleCount;
	}
}

void createBlackHole(PositionVector position) {
	if (blackHoleCount < MAX_BLACKHOLES) {
		blackHoleArray[blackHoleCount] = initBlackHole(position);
		++blackHoleCount;
	}
}

// Gameplay
void gameOver() {
	// Free asteroids
	for (int i = 0; i < asteroidCount; ++i) {
		destructAsteroid(asteroidArray[i]);
	}
	asteroidCount = 0;

	// Free bullets
	for (int i = 0; i < bulletCount; ++i) {
		destructBullet(bulletArray[i]);
	}
	bulletCount = 0;

	// Free player
	destructPlayer(playerData);
	playerData = NULL;

	// Free particles
	for (int i = 0; i < particleCount; ++i) {
		destructParticle(particleArray[i]);
	}
	particleCount = 0;

	// Free blackholes
	for (int i = 0; i < blackHoleCount; ++i) {
		destructBlackHole(blackHoleArray[i]);
	}
	blackHoleCount = 0;

	// Reset game over key
	inputDuringGameOver = 0;

	gameState = GAME_OVER;
}

// Collision Detection
void checkCollisions() {
	// Check collision with asteroids
	// Player / Asteroid collisions
	// Bullet / Asteroid collisions
	// Asteroid / Wall collisions
	// BlackHole / Asteroid collisions
	asteroidCollisions();

	// check collisions with blackholes
	// Player / BlackHole
	// Bullets / BlackHole
	// Particles / BackHole
	blackHoleCollision();
}

// This is the heaviest part of the code base, its essentially the entire game, it could be heavily modified to be made faster
// however that is out of the scope of this project, i will however attempt to do as much optimisation in the time i have available
void asteroidCollisions() {
	for (int i = 0; i < asteroidCount; ++i) {
		// Get distance between asteroid and player
		float distance = vectorLength(subtractVectors(playerData->position, asteroidArray[i]->position));
		
		// Check for collision circle overlap for player
		if (distance < playerData->collisionRadius + asteroidArray[i]->collisionRadius) {
			gameOver();
		}

		// Check for collisions with other asteroids
		for (int j = 0; j < asteroidCount; ++j) {
			if (i != j) {
				// Distance between asteroids
				PositionVector distanceVector = subtractVectors(asteroidArray[i]->position, asteroidArray[j]->position);
				float distance = vectorLength(distanceVector);
				if (distance < asteroidArray[i]->collisionRadius + asteroidArray[j]->collisionRadius) {
					if (asteroidArray[i]->inside == 1 && asteroidArray[j]->inside == 1) {
						PositionVector temp;
						temp.x = asteroidArray[i]->moveVector.x;
						temp.y = asteroidArray[i]->moveVector.y;
						asteroidArray[i]->moveVector.x = asteroidArray[j]->moveVector.x;
						asteroidArray[i]->moveVector.y = asteroidArray[j]->moveVector.y;
						asteroidArray[j]->moveVector.x = temp.x;
						asteroidArray[j]->moveVector.y = temp.y;
					}
					else {
						// if an asteroid in the collision is outside then reflect each other based on collision vector
						asteroidArray[i]->moveVector = addVectors(asteroidArray[i]->moveVector, distanceVector);
						asteroidArray[j]->moveVector = subtractVectors(asteroidArray[j]->moveVector, distanceVector);
					}

					// Move the asteroids apart so they arent within each other
					// the distance of overlap between the two asteroids is equal to the distance between there centres minus the non overlapping distance
					// here i calculate the vector of overlap and divide it in two to move the asteroids apart equally
					// this is to prevent asteroid overlap
					PositionVector distanceUnitVector = vectorToUnitVector(distanceVector);
					PositionVector asteroid1OverlapVector = multiplyVector(distanceUnitVector, asteroidArray[i]->collisionRadius);
					PositionVector asteroid2OverlapVector = multiplyVector(distanceUnitVector, asteroidArray[j]->collisionRadius);
					PositionVector overlapVector = subtractVectors(subtractVectors(distanceVector, asteroid1OverlapVector), asteroid2OverlapVector);
					PositionVector displacementVector = multiplyVector(overlapVector, 0.5);
					asteroidArray[i]->position = subtractVectors(asteroidArray[i]->position, displacementVector);
					asteroidArray[j]->position = addVectors(asteroidArray[j]->position, displacementVector);
				}
			}
		}

		// Check for collisions with black holes
		if (collidingWithBlackHole(asteroidArray[i]->position, asteroidArray[i]->collisionRadius) > 0) {
			explodeAsteroid(asteroidArray[i], i);
		}

		// Check for collisions with bullets
		for (int j = 0; j < bulletCount; ++j) {
			float distance = vectorLength(subtractVectors(bulletArray[j]->position, asteroidArray[i]->position));

			if (distance < asteroidArray[i]->collisionRadius) {
				// despawn bullet
				removeBullet(bulletArray[j], j);

				// remove health from asteroid
				asteroidArray[i]->hp -= PLAYER_DAMAGE_DEALT;
				if (asteroidArray[i]->hp <= 0) {
					explodeAsteroid(asteroidArray[i], i);
				}
			}
		} 
	}
}

void blackHoleCollision() {
	// player collision
	if (playerData != NULL && collidingWithBlackHole(playerData->position, playerData->collisionRadius) > 0) {
		gameOver();
	}

	// bullet collision
	for (int i = 0; i < bulletCount; ++i) {
		if (collidingWithBlackHole(bulletArray[i]->position, 0) > 0) {
			removeBullet(bulletArray[i], i);
		}
	}

	// particle collision
	for (int i = 0; i < particleCount; ++i) {
		if (collidingWithBlackHole(particleArray[i]->position, 0) > 0) {
			particleArray[i]->size = 0;
		}
	}
}

int collidingWithBlackHole(PositionVector position, float radius) {
	// Check for collisions with black holes
	int colliding = 0;
	for (int j = 0; j < blackHoleCount; ++j) {
		float distance = vectorLength(subtractVectors(blackHoleArray[j]->position, position));

		if (distance < radius + blackHoleArray[j]->radius) {
			colliding = 1;
		}
	}
	return colliding;
}

// Asteroid Actions
void explodeAsteroid(Asteroid* asteroid, int index) {
	// Spawn smaller asteroids if size > 1
	if (asteroid->size > 1) {
		splitAsteroid(asteroid);
	}

	// Create asteroid puff effect
	for (int i = 0; i < ASTEROID_PUFF_COUNT; ++i) {
		createParticle(asteroid->position, multiplyVector(angleToUnitVector((rand() % 100 / 100) * 2 * PI), 0.1), ASTEROID_PUFF_SIZE);
	}

	// Despawn Asteroid
	removeAsteroid(asteroid, index);
}

void splitAsteroid(Asteroid* asteroid) {
	// If there is room for more asteroids then split new asteroids
	for (int i = 0; asteroidCount < MAX_ASTEROIDS && i < 2; ++i) {
		// Spawn new split asteroid
		asteroidArray[asteroidCount] = initAsteroid(asteroid->position.x + (1 - i * 2) * ASTEROID_SPLIT_RADIUS_MULTIPLIER, asteroid->position.y);
		asteroidArray[asteroidCount]->moveVector = rotateVector(asteroid->moveVector, ASTEROID_SPLIT_ANGLE, (-1 + i * 2));
		asteroidArray[asteroidCount]->size = asteroid->size - 1;
		++asteroidCount;
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

// Movement
PositionVector movePosition(float delta, PositionVector position, PositionVector movement) {
	movement.x *= delta;
	movement.y *= delta;
	return addVectors(position, movement);
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

// Asteroid movement
void moveAsteroid(float delta, Asteroid* asteroid) {
	asteroid->position = movePosition(delta, asteroid->position, asteroid->moveVector);

	// Spin
	asteroid->angle += asteroid->spin * delta;

	// Accelerate towards player if outside the arena
	if (asteroid->inside == 0) {
		PositionVector distanceVector = subtractVectors(playerData->position, asteroid->position);
		PositionVector acceleration = multiplyVector(distanceVector, ASTEROID_SPAWN_ACCELERATION_MULTIPLIER * delta);
		asteroid->moveVector = addVectors(asteroid->moveVector, acceleration);
	}
}

void removeAsteroid(Asteroid* asteroid, int index) {
	// Remove asteroid from array and shuffle down asteroids
	for (int i = index; i < asteroidCount - 1; ++i) {
		asteroidArray[i] = asteroidArray[i + 1];
	}
	asteroidArray[asteroidCount] = NULL;
	--asteroidCount;

	// Destruct bullet
	destructAsteroid(asteroid);
}

// Player Movement
void movePlayer(float delta) {
	playerData->position = movePosition(delta, playerData->position, playerData->moveVector);
}

void rotatePlayer(float delta) {
	int dir = key_left - key_right;

	playerData->direction = rotateVector(playerData->direction, DEGREE_OF_ROTATION * delta, dir);
}

void acceleratePlayer(float delta, int dir) {
	// Accelerate player velocity towards player direction
	// Only accelerate when below max velocity
	if (dir > 0 && vectorLength(playerData->moveVector) <= playerData->maxVelocity) {
		PositionVector accelerate;

		accelerate.x = playerData->direction.x * playerData->acceleration * dir * delta;
		accelerate.y = playerData->direction.y * playerData->acceleration * dir * delta;
		PositionVector newMoveVector = addVectors(playerData->moveVector, accelerate);
		
		// slow ship to max velocity
		if (vectorLength(newMoveVector) < playerData->maxVelocity) {
			playerData->moveVector = newMoveVector;
		}

		// Create engine particles
		if (playerData->particleCoolDown <= 0) {
			createParticle(playerData->position, playerData->direction, PLAYER_PARTICLE_SIZE);
		}
	}
}

void fireCannonPlayer(float delta) {
	// Create bullet
	if (playerData->cannonCoolDown <= 0 && bulletCount < MAX_BULLETS && playerData->firing == 1) {
		playerData->cannonCoolDown = PLAYER_CANNON_COOLDOWN;
		bulletArray[bulletCount] = initBullet(playerData);
		++bulletCount;
	}

	// reduce cooldown for players cannon
	if (playerData->cannonCoolDown > 0) {
		playerData->cannonCoolDown -= delta;
	}
}

// Bullet Movement
void moveBullets(float delta) {
	for (int i = 0; i < bulletCount; ++i) {
		Bullet* bullet = bulletArray[i];
		bullet->position = movePosition(delta, bullet->position, bullet->moveVector);
		// Theres an issue here, cullBullet removes the bullet struct before movebullets is complete, potentially skipping a bullet
		// or worse, potentially causing undefined behaviour
		cullBullet(bullet, i);
	}
}

void cullBullet(Bullet* bullet, int index) {
	if (boolOutOfBounds(bullet->position, 32) > 0) {
		removeBullet(bullet, index);
	}
}

void removeBullet(Bullet* bullet, int index) {
	// Remove bullet from array and shuffle down bullets
	for (int i = index; i < bulletCount - 1; ++i) {
		bulletArray[i] = bulletArray[i + 1];
	}
	bulletArray[bulletCount] = NULL;
	--bulletCount;

	// Destruct bullet
	destructBullet(bullet);
}

// Engine Particle Movement
void moveParticles(float delta) {
	for (int i = 0; i < particleCount; ++i) {
		// Move particle
		Particle* particle = particleArray[i];
		particle->position = movePosition(delta, particle->position, particle->moveVector);

		// Spin particle
		particle->angle += particle->spin * delta;
		if (particle->angle >= 2 * PI) {
			particle->angle -= 2 * PI;
		}
		else if (particle->angle <= 0) {
			particle->angle += 2 * PI;
		}

		// Theres an issue here, cullEngineParticle removes the engine particle struct before movebullets is complete, potentially skipping a bullet
		// or worse, potentially causing undefined behaviour
		// Decrease size
		particle->size -= particle->decaySpeed * delta;
		if (particle->size <= 0) {
			cullParticle(particle, i);
		}
	}
}

void cullParticle(Particle* particle, int index) {
	for (int i = index; i < particleCount - 1; ++i) {
		particleArray[i] = particleArray[i + 1];
	}
	particleArray[particleCount] = NULL;
	--particleCount;

	destructParticle(particle);
}

void applyGravity(float delta) {
	// Apply Gravity from black holes
	for (int i = 0; i < blackHoleCount; ++i) {
		// Apply gravity to asteroids
		for (int j = 0; j < asteroidCount; ++j) {
			PositionVector acceleration = getAccelerationVector(delta, blackHoleArray[i], asteroidArray[j]->position);
			asteroidArray[j]->moveVector = addVectors(asteroidArray[j]->moveVector, acceleration);
		}

		// Apply gravity for player
		PositionVector acceleration = getAccelerationVector(delta, blackHoleArray[i], playerData->position);
		playerData->moveVector = addVectors(playerData->moveVector, acceleration);

		// Apply gravity to bullets
		for(int j = 0; j < bulletCount; ++j) {
			PositionVector acceleration = getAccelerationVector(delta, blackHoleArray[i], bulletArray[j]->position);
			bulletArray[j]->moveVector = addVectors(bulletArray[j]->moveVector, acceleration);
		}

		// Apply gravity to particles
		for (int j = 0; j < particleCount; ++j) {
			PositionVector acceleration = getAccelerationVector(delta, blackHoleArray[i], particleArray[j]->position);
			particleArray[j]->moveVector = addVectors(particleArray[j]->moveVector, acceleration);
		}
	}
}

PositionVector getAccelerationVector(float delta, BlackHole* bh, PositionVector objectPosition) {
	PositionVector distanceVector = subtractVectors(bh->position, objectPosition);
	// apply inverse square law to gravity acceleration
	float pull = (bh->gravity / pow(vectorLength(distanceVector), 2)) * delta;
	return multiplyVector(vectorToUnitVector(distanceVector), pull);
}

// Object Rendering
void renderAsteroids() {
	// Create the asteroid in OpenGl
	for (int i = 0; i < asteroidCount; ++i) {
		float x = asteroidArray[i]->position.x;
		float y = asteroidArray[i]->position.y;
		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((asteroidArray[i]->angle * 180.0 / PI) - 90, 0.0, 0.0, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);
		
		drawAsteroid(asteroidArray[i]->vertices, asteroidArray[i]->vertexCount, asteroidArray[i]->size);

		glPopMatrix();
	}
}

void renderPlayer() {
	if (playerData != NULL) {
		float x = playerData->position.x;
		float y = playerData->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((vectorAngle(playerData->direction) * 180.0/PI) - 90, 0.0, 0.0, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);
		
		drawPlayer();

		glPopMatrix();
	}
}

void renderBullets() {
	for (int i = 0; i < bulletCount; ++i) {
		float x = bulletArray[i]->position.x;
		float y = bulletArray[i]->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);
		
		drawBullet();

		glPopMatrix();
	}
}

void renderEngineParticles() {
	for (int i = 0; i < particleCount; ++i) {
		float x = particleArray[i]->position.x;
		float y = particleArray[i]->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((particleArray[i]->angle * 180.0 / PI) - 90, 0, 0, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawPentagon(particleArray[i]->size);

		glPopMatrix();
	}
}

void renderBlackHoles() {
	for (int i = 0; i < blackHoleCount; ++i) {
		glPushMatrix();
		glTranslatef(blackHoleArray[i]->position.x, blackHoleArray[i]->position.y, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawBlackHole(blackHoleArray[i]);

		glPopMatrix();
	}
}
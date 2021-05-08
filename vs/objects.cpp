/*	
*	elements contains functionality for object data storage and rendering
* 
*	Main Author: Dean
*/

#include "objects.h"

// Object Data
Asteroid* asteroidArray[MAX_ASTEROIDS];
int asteroidCount = 0;

Bullet* bulletArray[MAX_BULLETS];
int bulletCount = 0;

Player* playerData = NULL;

Particle* particleArray[MAX_ENGINE_PARTICLES];
int particleCount = 0;

Bullet* initBullet() {
	Bullet* bullet = new Bullet;
	return bullet;
}

Particle* initParticle(PositionVector position, PositionVector direction, int maxSize) {
	Particle* particle = new Particle;

	// positon = player position + randomness - player direction
	// player direction is subtracted to spawn particles at the back of the ship
	particle->position.x = position.x + (rand() % 10 - 5) - direction.x * 10;
	particle->position.y = position.y + (rand() % 10 - 5) - direction.y * 10;

	// velocity is based on the inverse direction of acceleration of the player + some randomness
	particle->moveVector.x = -direction.x * 1000 + rand() % 200;
	particle->moveVector.y = -direction.y * 1000 + rand() % 200;
	particle->decaySpeed = rand() % (PARTICLE_DECAY_SPEED / 2) + PARTICLE_DECAY_SPEED;
	particle->size = rand() % maxSize;
	particle->spin = rand() % 5 - 2.5;
	particle->angle = PI / 2;
	return particle;
}

// Object Creation
void createAsteroid(float x, float y) {
	if (asteroidCount < MAX_ASTEROIDS) {
		Asteroid* asteroid = new Asteroid(x, y);
		asteroidArray[asteroidCount] = asteroid;
		++asteroidCount;
	}
}

void createPlayer(float x, float y) {
	freePlayer();
	playerData = new Player(x, y);
}

void createParticle(PositionVector position, PositionVector velocity, int maxSize) {
	if (particleCount < MAX_ENGINE_PARTICLES) {
		Particle* particle = initParticle(position, velocity, maxSize);
		particleArray[particleCount] = particle;
		++particleCount;
	}
}

void createBullet(PositionVector position, PositionVector direction) {
	if (bulletCount < MAX_BULLETS) {
		Bullet* bullet = initBullet();

		// Initialise bullet variables
		bullet->position.x = position.x;
		bullet->position.y = position.y;
		bullet->moveVector.x = direction.x * BULLET_VECOLITY;
		bullet->moveVector.y = direction.y * BULLET_VECOLITY;

		// Add bullet to array
		bulletArray[bulletCount] = bullet;
		++bulletCount;
	}
}

// Object Access
Player* getPlayer() {
	return playerData;
}

Asteroid* getAsteroid(int index) {
	Asteroid* asteroid = NULL;
	if (index >= 0 && index < asteroidCount) {
		asteroid = asteroidArray[index];
	}
	return asteroid;
}

void pushAsteroid(Asteroid* asteroid) {
	if (asteroidCount < MAX_ASTEROIDS && asteroid != NULL) {
		asteroidArray[asteroidCount] = asteroid;
		++asteroidCount;
	}
}

int getAsteroidCount() {
	return asteroidCount;
}

Particle* getParticle(int index) {
	return particleArray[index];
}

int getParticleCount() {
	return particleCount;
}

Bullet* getBullet(int index) {
	return bulletArray[index];
}

int getBulletCount() {
	return bulletCount;
}

// Object Destruction
void freeAllObjects() {
	// Free all game objects
	freeAsteroids();
	freeBullets();
	freeParticles();
	freeBlackHoles();
	freePlayer();
}

void freeAsteroids() {
	// Free asteroids
	for (int i = 0; i < asteroidCount; ++i) {
		destructAsteroid(asteroidArray[i]);
	}
	asteroidCount = 0;
}

void freeBullets() {
	// Free bullets
	for (int i = 0; i < bulletCount; ++i) {
		destructBullet(bulletArray[i]);
	}
	bulletCount = 0;
}

void freeParticles() {
	// Free particles
	for (int i = 0; i < particleCount; ++i) {
		destructParticle(particleArray[i]);
	}
	particleCount = 0;
}

void freePlayer() {
	if (playerData) {
		destructPlayer(playerData);
		playerData = NULL;
	}
}

void freeAsteroid(int index) {
	if (index >= 0 && index < asteroidCount) {
		Asteroid* asteroid = asteroidArray[index];

		// Shuffle array
		for (int i = index; i < asteroidCount - 1; ++i) {
			asteroidArray[i] = asteroidArray[i + 1];
		}
		--asteroidCount;

		// Free asteroid
		destructAsteroid(asteroid);
	}
}

void freeBullet(int index) {
	if (index >= 0 && index < bulletCount) {
		Bullet* bullet = bulletArray[index];

		// Shuffle array
		for (int i = index; i < bulletCount - 1; ++i) {
			bulletArray[i] = bulletArray[i + 1];
		}
		--bulletCount;

		// Free bullet
		destructBullet(bullet);
	}
}

void freeParticle(int index) {
	if (index >= 0 && index < particleCount) {
		Particle* particle = particleArray[index];

		// Shuffle array
		for (int i = index; i < particleCount - 1; ++i) {
			particleArray[i] = particleArray[i + 1];
		}
		--particleCount;

		// Free particle
		destructParticle(particle);
	}
}

void destructAsteroid(Asteroid* asteroid) {
	delete asteroid;
}

void destructPlayer(Player* player) {
	delete player;
}

void destructBullet(Bullet* bullet) {
	free(bullet);
}

void destructParticle(Particle* particle) {
	free(particle);
}
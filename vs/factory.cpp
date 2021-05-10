/*	
*	elements contains functionality for object data storage and rendering
* 
*	Main Author: Dean
*/

#include "factory.h"

// Object Data
std::vector<Asteroid*> asteroidArray;

std::vector<Bullet*> bulletArray;

Player* player = NULL;

std::vector<Particle*> particleArray;

// Object Creation
void createAsteroid(float x, float y) {
	asteroidArray.push_back(new Asteroid(x, y));
}

void createPlayer(float x, float y) {
	freePlayer();
	player = new Player(x, y);
}

void createParticle(PositionVector position, PositionVector moveVector) {
	particleArray.push_back(new Particle(position, moveVector));
}

void createBullet(PositionVector position, PositionVector direction) {
	bulletArray.push_back(new Bullet(position, multiplyVector(direction, BULLET_VECOLITY)));
}

// Object Access
Player* getPlayer() {
	return player;
}

Asteroid* getAsteroid(int index) {
	Asteroid* asteroid = NULL;
	if (index >= 0 && index < asteroidArray.size()) {
		asteroid = asteroidArray.at(index);
	}
	return asteroid;
}

void pushAsteroid(Asteroid* asteroid) {
	if (asteroid != NULL) {
		asteroidArray.push_back(asteroid);
	}
}

int getAsteroidCount() {
	return asteroidArray.size();
}

Particle* getParticle(int index) {
	Particle* particle = NULL;
	if (index >= 0 && index < particleArray.size()) {
		particle = particleArray.at(index);
	}
	return particle;
}

int getParticleCount() {
	return particleArray.size();
}

Bullet* getBullet(int index) {
	return bulletArray.at(index);
}

int getBulletCount() {
	return bulletArray.size();
}

// Object Destruction
void freeAllObjects() {
	// Free all game objects
	freeAsteroids();
	freeBullets();
	freeParticles();
	freePlayer();
}

void freeAsteroids() {
	// Destruct all asteroids
	asteroidArray.clear();
}

void freeBullets() {
	// Destruct all bullets
	bulletArray.clear();
}

void freeParticles() {
	// Destruct all particles
	particleArray.clear();
}

void freePlayer() {
	if (player) {
		delete player;
		player = NULL;
	}
}

void freeAsteroid(int index) {
	if (index >= 0 && index < asteroidArray.size()) {
		asteroidArray.erase(asteroidArray.begin() + index);
	}
}

void freeBullet(int index) {
	if (index >= 0 && index < bulletArray.size()) {
		bulletArray.erase(bulletArray.begin() + index);
	}
}

void freeParticle(int index) {
	if (index >= 0 && index < particleArray.size()) {
		particleArray.erase(particleArray.begin() + index);
	}
}
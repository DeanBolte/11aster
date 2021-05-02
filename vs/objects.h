/*
*	elements contains functionality for object data storage and rendering
*
*	Main Author: Dean
*/
#pragma once

#include "util.h"

#define MAX_ASTEROIDS 500
#define MAX_BULLETS 100
#define MAX_ENGINE_PARTICLES 1000
#define MAX_BLACKHOLES 3

#define BULLET_VECOLITY 1000.0

#define ASTEROID_SPIN 6
#define ASTEROID_VERTEX_COUNT 8
#define ASTEROID_BASE_SIZE 16
#define ASTEROID_VARIANCE 4
#define MAX_ASTEROID_VARIANCE 4
#define ASTEROID_HP_MULTIPLIER 3;

#define PLAYER_COLLISION_RADIUS 20.0
#define PLAYER_CANNON_COOLDOWN 0.2
#define PLAYER_MAXIMUM_VELOCITY 1000.0
#define PLAYER_MINIMUM_VELOCITY 10
#define PLAYER_ACCELERATION 1200.0
#define PLAYER_DAMAGE_DEALT 10;
#define PLAYER_PARTICLE_INTERVAL 0.001

#define PARTICLE_DECAY_SPEED 30

#define BLACKHOLE_RADIUS 20
#define GRAVITY_MULTIPLIER 2000000

typedef struct Asteroid {
	PositionVector position;
	PositionVector moveVector;
	PositionVector* vertices;
	int inside;
	float collisionRadius;
	int vertexCount;
	int size;
	int hp;
	float spin;
	float angle;
} Asteroid;

typedef struct Player {
	PositionVector position;
	PositionVector moveVector;
	PositionVector direction;
	int hp;
	float particleCoolDown;
	float cannonCoolDown;
	float collisionRadius;
	float acceleration;
	float maxVelocity;
	float minVelocity;
} Player;

typedef struct Bullet {
	PositionVector position;
	PositionVector moveVector;
} Bullet;

typedef struct Particle {
	PositionVector position;
	PositionVector moveVector;
	float decaySpeed;
	float spin;
	float angle;
	float size;
} Particle;

typedef struct BlackHole {
	PositionVector position;
	float radius;
	float radiusOutPulse;
	float radiusInPulse;
	float gravity;
	int pulseDirection;
} BlackHole;

// Colours
RGB highColour;
RGB lowColour;
RGB warnColour;

// Object Initialisation
Asteroid* initAsteroid(float x, float y);
Player* initPlayer(float x, float y);
Bullet* initBullet();
Particle* initParticle(PositionVector position, PositionVector direction, int maxSize);
BlackHole* initBlackHole(PositionVector position);

// Object Creation
void createAsteroid(float x, float y);
void createPlayer(float x, float y);
void createParticle(PositionVector position, PositionVector velocity, int maxSize);
void createBlackHole(PositionVector position);
void createBullet(PositionVector position, PositionVector direction);

// Object Access
Player* getPlayer();
Asteroid* getAsteroid(int index);
void pushAsteroid(Asteroid* asteroid);
int getAsteroidCount();
Particle* getParticle(int index);
int getParticleCount();
Bullet* getBullet(int index);
int getBulletCount();
BlackHole* getBlackHole(int index);
int getBlackHoleCount();

// Object Destruction
void freeAllObjects();
void freeAsteroids();
void freeBullets();
void freeParticles();
void freeBlackHoles();
void freePlayer();
void freeAsteroid(int index);
void freeBullet(int index);
void freeParticle(int index);
void freeBlackHole(int index);
void destructAsteroid(Asteroid* asteroid);
void destructPlayer(Player* player);
void destructBullet(Bullet* bullet);
void destructParticle(Particle* particle);
void destructBlackHole(BlackHole* bh);
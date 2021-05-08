/*
*	elements contains functionality for object data storage and rendering
*
*	Main Author: Dean
*/
#pragma once

#include "util.h"
#include "player.h"
#include "asteroid.h"
#include "bullet.h"
#include "particle.h"

#define MAX_ASTEROIDS 500
#define MAX_BULLETS 100
#define MAX_ENGINE_PARTICLES 1000
#define MAX_BLACKHOLES 

#define BULLET_VECOLITY 1000.0

#define BLACKHOLE_RADIUS 20
#define GRAVITY_MULTIPLIER 2000000

// Object Creation
void createAsteroid(float x, float y);
void createPlayer(float x, float y);
void createParticle(PositionVector position, PositionVector moveVector);
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

// Object Destruction
void freeAllObjects();
void freeAsteroids();
void freeBullets();
void freeParticles();
void freePlayer();
void freeAsteroid(int index);
void freeBullet(int index);
void freeParticle(int index);
void destructAsteroid(Asteroid* asteroid);
void destructPlayer(Player* player);
void destructBullet(Bullet* bullet);
void destructParticle(Particle* particle);
/*
*	the player class represents the data for the player
*
*	Main Author: Dean
*/
#pragma once

#include "util.h"

#define PLAYER_COLLISION_RADIUS 20.0f
#define PLAYER_CANNON_COOLDOWN 0.2f
#define PLAYER_MAXIMUM_VELOCITY 1000.0f
#define PLAYER_MINIMUM_VELOCITY 10.0f
#define PLAYER_ACCELERATION 1200.0f
#define PLAYER_DAMAGE_DEALT 10
#define PLAYER_DAMAGE_TAKEN 10
#define PLAYER_PARTICLE_INTERVAL 0.001f
#define PLAYER_PARTICLE_SIZE 5
#define PLAYER_BRAKE_MULTIPLIER 2

#define DEGREE_OF_ROTATION 2 * PI

class Player {
private:
	// Body
	PositionVector position;
	PositionVector moveVector;
	PositionVector direction;

	// Health
	int hp;
	int maxHp;
	
	// Collision
	float collisionRadius;

	// Movement
	float acceleration;
	float maxVelocity;
	float minVelocity;

	// Cooldowns
	float particleCoolDown;
	float cannonCoolDown;

	// Update functions
	void move(float delta);

public:
	// Constructors
	Player(float x, float y);

	// Update calls
	void update(float delta);

	// Movement
	PositionVector getPosition();
	PositionVector getMoveVector();
	PositionVector getDirection();
	void rotatePlayer(float degree);
	void acceleratePlayer(float delta, int dir);
	void brakePlayer(float delta);
	
	// Health
	int getHp();
	void incrementHp(int increment);
	int getMaxHp();
	
	float getCollisionRadius();
	float getAcceleration();
	float getMaxVelocity();
	float GetMinVelocity();

	// Cooldowns
	float getParticleCoolDown();
	void incrementParticleCoolDown(float increment);
	float getCannonCoolDown();
	void incrementCannonCoolDown(float increment);
	
};
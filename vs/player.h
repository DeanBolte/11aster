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
#define PLAYER_PARTICLE_INTERVAL 0.001f

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

public:
	// Constructors
	Player(float x, float y);

	// Access
	PositionVector getPosition();
	PositionVector getMoveVector();
	PositionVector getDirection();
	
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
/*
*	the player class represents the data for the player
*
*	Main Author: Dean
*/
#pragma once

#include "util.h"

#define PLAYER_COLLISION_RADIUS 20.0
#define PLAYER_CANNON_COOLDOWN 0.2
#define PLAYER_MAXIMUM_VELOCITY 1000.0
#define PLAYER_MINIMUM_VELOCITY 10
#define PLAYER_ACCELERATION 1200.0
#define PLAYER_DAMAGE_DEALT 10;
#define PLAYER_PARTICLE_INTERVAL 0.001

class Player {
private:
	PositionVector position;
	PositionVector moveVector;
	PositionVector direction;
	int hp;
	int maxHp;
	float particleCoolDown;
	float cannonCoolDown;
	float collisionRadius;
	float acceleration;
	float maxVelocity;
	float minVelocity;

public:
	// Constructors
	Player(float x, float y);
	
};
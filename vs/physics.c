/*
*	physics contains functionality for movement and collision of objects
*
*	Main Contributer: Dean
*/

#include "physics.h"

//		Physics Engine Calls
void physicsMovements(float delta) {
	// Apply gravity from black holes
	// Applies to Asteroids, Bullets, particles and the Player
	//applyGravity(delta);

	// Update asteroid positions
	for (int i = 0; i < getAsteroidCount(); ++i) {
		moveAsteroid(delta, getAsteroid(i));
	}

	// Update player position
	movePlayer(delta);

	// Update bullet positions
	moveBullets(delta);

	// Update particle positions
	moveParticles(delta);
}

void physicsCollisions() {
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

//		Collision Detection
// This is the heaviest part of the code base, its essentially the entire game, it could be heavily modified to be made faster
// however that is out of the scope of this project, i will however attempt to do as much optimisation in the time i have available
void asteroidCollisions() {
	for (int i = 0; i < getAsteroidCount(); ++i) {
		// Get distance between asteroid and player
		float distance = vectorLength(subtractVectors(getPlayer()->position, getAsteroid(i)->position));

		// Check for collision circle overlap for player
		if (distance < getPlayer()->collisionRadius + getAsteroid(i)->collisionRadius) {
			gameOver();
		}

		// Check for collisions with other asteroids
		for (int j = 0; j < getAsteroidCount(); ++j) {
			if (i != j) {
				// Distance between asteroids
				PositionVector distanceVector = subtractVectors(getAsteroid(i)->position, getAsteroid(j)->position);
				float distance = vectorLength(distanceVector);
				if (distance < getAsteroid(i)->collisionRadius + getAsteroid(j)->collisionRadius) {
					if (getAsteroid(i)->inside == 1 && getAsteroid(j)->inside == 1) {
						PositionVector temp;
						temp.x = getAsteroid(i)->moveVector.x;
						temp.y = getAsteroid(i)->moveVector.y;
						getAsteroid(i)->moveVector.x = getAsteroid(j)->moveVector.x;
						getAsteroid(i)->moveVector.y = getAsteroid(j)->moveVector.y;
						getAsteroid(j)->moveVector.x = temp.x;
						getAsteroid(j)->moveVector.y = temp.y;
					}
					else {
						// if an asteroid in the collision is outside then reflect each other based on collision vector
						getAsteroid(i)->moveVector = addVectors(getAsteroid(i)->moveVector, distanceVector);
						getAsteroid(j)->moveVector = subtractVectors(getAsteroid(j)->moveVector, distanceVector);
					}

					// Move the asteroids apart so they arent within each other
					// the distance of overlap between the two asteroids is equal to the distance between there centres minus the non overlapping distance
					// here i calculate the vector of overlap and divide it in two to move the asteroids apart equally
					// this is to prevent asteroid overlap
					PositionVector distanceUnitVector = vectorToUnitVector(distanceVector);
					PositionVector asteroid1OverlapVector = multiplyVector(distanceUnitVector, getAsteroid(i)->collisionRadius);
					PositionVector asteroid2OverlapVector = multiplyVector(distanceUnitVector, getAsteroid(j)->collisionRadius);
					PositionVector overlapVector = subtractVectors(subtractVectors(distanceVector, asteroid1OverlapVector), asteroid2OverlapVector);
					PositionVector displacementVector = multiplyVector(overlapVector, 0.5);
					getAsteroid(i)->position = subtractVectors(getAsteroid(i)->position, displacementVector);
					getAsteroid(j)->position = addVectors(getAsteroid(j)->position, displacementVector);
				}
			}
		}

		// Check for collisions with black holes
		if (collidingWithBlackHole(getAsteroid(i)->position, getAsteroid(i)->collisionRadius) > 0) {
			explodeAsteroid(getAsteroid(i), i);
		}

		// Check for collisions with bullets
		for (int j = 0; j < getBulletCount(); ++j) {
			float distance = vectorLength(subtractVectors(getBullet(j)->position, getAsteroid(i)->position));

			if (distance < getAsteroid(i)->collisionRadius) {
				// despawn bullet
				freeBullet(j);

				// remove health from asteroid
				getAsteroid(i)->hp -= PLAYER_DAMAGE_DEALT;
				if (getAsteroid(i)->hp <= 0) {
					explodeAsteroid(getAsteroid(i), i);
				}
			}
		}
	}
}

void blackHoleCollision() {
	// player collision
	if (getPlayer() != NULL && collidingWithBlackHole(getPlayer()->position, getPlayer()->collisionRadius) > 0) {
		gameOver(); // replace with player health and game over in update within game engine
	}

	// bullet collision
	for (int i = 0; i < getBulletCount(); ++i) {
		if (collidingWithBlackHole(getBullet(i)->position, 0) > 0) {
			freeBullet(i);
		}
	}

	// particle collision
	for (int i = 0; i < getParticleCount(); ++i) {
		if (collidingWithBlackHole(getParticle(i)->position, 0) > 0) {
			getParticle(i)->size = 0;
		}
	}
}

int collidingWithBlackHole(PositionVector position, float radius) {
	// Check for collisions with black holes
	int colliding = 0;
	for (int i = 0; i < getBlackHoleCount(); ++i) {
		float distance = vectorLength(subtractVectors(getBlackHole(i)->position, position));

		if (distance < radius + getBlackHole(i)->radius) {
			colliding = 1;
		}
	}
	return colliding;
}

// Asteroid Physics
void explodeAsteroid(int index) {
	// Spawn smaller asteroids if size > 1
	if (getAsteroid(index)->size > 1) {
		splitAsteroid(getAsteroid(index));
	}

	// Create asteroid puff effect
	for (int i = 0; i < ASTEROID_PUFF_COUNT; ++i) {
		createParticle(getAsteroid(index)->position, multiplyVector(angleToUnitVector((rand() % 100 / 100) * 2 * PI), 0.1), ASTEROID_PUFF_SIZE);
	}

	// Despawn Asteroid
	freeAsteroid(index);
}

void splitAsteroid(Asteroid* asteroid) {
	// If there is room for more asteroids then split new asteroids
	for (int i = 0; getAsteroidCount() < MAX_ASTEROIDS && i < 2; ++i) {
		// Spawn new split asteroid
		pushAsteroid(initAsteroid(asteroid->position.x + (1 - i * 2) * ASTEROID_SPLIT_RADIUS_MULTIPLIER, asteroid->position.y));
		getAsteroid(getAsteroidCount() - 1)->moveVector = rotateVector(asteroid->moveVector, ASTEROID_SPLIT_ANGLE, (-1 + i * 2));
		getAsteroid(getAsteroidCount() - 1)->size = asteroid->size - 1;
	}
}

// Movement
PositionVector movePosition(float delta, PositionVector position, PositionVector movement) {
	movement.x *= delta;
	movement.y *= delta;
	return addVectors(position, movement);
}

// Asteroid movement
void moveAsteroid(float delta, Asteroid* asteroid) {
	asteroid->position = movePosition(delta, asteroid->position, asteroid->moveVector);

	// Spin
	asteroid->angle += asteroid->spin * delta;
}

// Player Movement
void movePlayer(float delta) {
	getPlayer()->position = movePosition(delta, getPlayer()->position, getPlayer()->moveVector);
}

void rotatePlayer(float delta, int dir) {
	getPlayer()->direction = rotateVector(getPlayer()->direction, DEGREE_OF_ROTATION * delta, dir);
}

void acceleratePlayer(float delta, int dir) {
	// Accelerate player velocity towards player direction
	// Only accelerate when below max velocity
	if (dir > 0 && vectorLength(getPlayer()->moveVector) <= getPlayer()->maxVelocity) {
		PositionVector accelerate;

		accelerate.x = getPlayer()->direction.x * getPlayer()->acceleration * dir * delta;
		accelerate.y = getPlayer()->direction.y * getPlayer()->acceleration * dir * delta;
		PositionVector newMoveVector = addVectors(getPlayer()->moveVector, accelerate);

		// slow ship to max velocity
		if (vectorLength(newMoveVector) < getPlayer()->maxVelocity) {
			getPlayer()->moveVector = newMoveVector;
		}

		// Create engine particles
		if (getPlayer()->particleCoolDown <= 0) {
			createParticle(getPlayer()->position, getPlayer()->direction, PLAYER_PARTICLE_SIZE);
		}
	}
}

// Bullet Movement
void moveBullets(float delta) {
	for (int i = 0; i < getBulletCount(); ++i) {
		Bullet* bullet = getBullet(i);
		bullet->position = movePosition(delta, bullet->position, bullet->moveVector);
		// Theres an issue here, cullBullet removes the bullet struct before movebullets is complete, potentially skipping a bullet
		// or worse, potentially causing undefined behaviour
		cullBullet(bullet, i);
	}
}

// Engine Particle Movement
void moveParticles(float delta) {
	for (int i = 0; i < getParticleCount(); ++i) {
		// Move particle
		Particle* particle = getParticle(i);
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
			freeParticle(i);
		}
	}
}

void applyGravity(float delta) {
	// Apply Gravity from black holes
	for (int i = 0; i < getBlackHoleCount; ++i) {
		BlackHole* bh = getBlackHole(i);

		// Apply gravity to asteroids
		for (int j = 0; j < getAsteroidCount(); ++j) {
			PositionVector acceleration = getAccelerationVector(delta, bh, getAsteroid(j)->position);
			getAsteroid(j)->moveVector = addVectors(getAsteroid(j)->moveVector, acceleration);
		}

		// Apply gravity for player
		PositionVector acceleration = getAccelerationVector(delta, bh, getPlayer()->position);
		getPlayer()->moveVector = addVectors(getPlayer()->moveVector, acceleration);

		// Apply gravity to bullets
		for (int j = 0; j < getBulletCount(); ++j) {
			PositionVector acceleration = getAccelerationVector(delta, bh, getBullet(j)->position);
			getBullet(j)->moveVector = addVectors(getBullet(j)->moveVector, acceleration);
		}

		// Apply gravity to particles
		for (int j = 0; j < getParticleCount(); ++j) {
			PositionVector acceleration = getAccelerationVector(delta, bh, getParticle(j)->position);
			getParticle(j)->moveVector = addVectors(getParticle(j)->moveVector, acceleration);
		}
	}
}

PositionVector getAccelerationVector(float delta, BlackHole* bh, PositionVector objectPosition) {
	PositionVector distanceVector = subtractVectors(bh->position, objectPosition);
	// apply inverse square law to gravity acceleration
	float pull = (bh->gravity / pow(vectorLength(distanceVector), 2)) * delta;
	return multiplyVector(vectorToUnitVector(distanceVector), pull);
}
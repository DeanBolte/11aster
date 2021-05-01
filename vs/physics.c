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
	applyGravity(delta);

	// Update asteroid positions
	for (int i = 0; i < asteroidCount; ++i) {
		moveAsteroid(delta, asteroidArray[i]);
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

// Asteroid Physics
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
	freeAsteroid(index);
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
	playerData->position = movePosition(delta, playerData->position, playerData->moveVector);
}

void rotatePlayer(float delta, int dir) {
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
		for (int j = 0; j < bulletCount; ++j) {
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
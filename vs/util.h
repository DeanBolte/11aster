/*
*	uitl contains vector math functions and various other utility functions
*
*	Main Author: Dean
*/
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#	include <GL/glew.h>
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

#include "rgb.h"

#define PI 3.14159265

typedef struct PositionVector {
	float x, y;
} PositionVector;

// Vector math
PositionVector addVectors(PositionVector vector1, PositionVector vector2);
PositionVector subtractVectors(PositionVector vector1, PositionVector vector2);
PositionVector rotateVector(PositionVector vector, float radians, int dir);
PositionVector multiplyVector(PositionVector vector, float multiplier);

float vectorLength(PositionVector vector);
float vectorAngle(PositionVector vector);
PositionVector angleToUnitVector(float radians);
PositionVector vectorToUnitVector(PositionVector vector);

// Other Math
int intToCharacterCount(int number);
/*
*	A header file for the utilities source file
*
*	Author: Dean
*	Copyright: read description in main.c
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
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

#define PI 3.14159265

typedef struct PositionVector {
	float x, y;
} PositionVector;

typedef struct RGB {
	float r, g, b;
} RGB;

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
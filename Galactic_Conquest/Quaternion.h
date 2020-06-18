
#ifndef QUATERNION_H
#define QUATERNION_H

#include <GL/glut.h>
#include <stdio.h>
#include "Point.h"
#include "Vector.h"
#include "Matrix.h"

class Quaternion
	{ // class Quaternion
	public:
	// first three coordinates are imaginary parts 
	// last coordinate is real part
	GLfloat coords[4];

	// constructor: sets the quaternion to (0, 0, 0, 1)
	Quaternion();

	// constructor: sets the quaternion to (x, y, z, w)
	Quaternion(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

	// Set to a pure scalar value
	Quaternion(double scalar);

	// Set to a pure vector value
	Quaternion(Vector vector);
	
	// Set to a homogeneous point
	Quaternion(Point point);
	
	// Set to a rotation defined by a rotation matrix
	// WARNING: MATRIX MUST BE A VALID ROTATION MATRIX
	Quaternion(Matrix matrix);

	// Set to a rotation defined by an axis and angle
	void SetRotation(Vector axis, GLfloat theta);

	// Copy another Quaternion & return self
	Quaternion &operator = (Quaternion other);
	
	// Computes the norm (sum of squares)
	GLfloat Norm();
	
	// Reduce to unit quaternion
	Quaternion Unit();
	
	// Conjugate the quaternion
	Quaternion Conjugate();
	
	// Invert the quaternion
	Quaternion Inverse();

	// Scalar right-multiplication
	Quaternion operator *(GLfloat scalar);

	// Scalar right-division
	Quaternion operator /(GLfloat scalar);

	// Adds two quaternions together
	Quaternion operator +(Quaternion other);

	// Subtracts one quaternion from another
	Quaternion operator -(Quaternion other);

	// Multiplies two quaternions together
	Quaternion operator *(Quaternion other);

	// Acts on a vector
	Vector Act(Vector vector);
	
	// Acts on a homogeneous point
	Point Act(Point point);
	
	// Returns the angle 2*theta of the action in degrees
	GLfloat AngleOfAction();
	
	// Returns the axis of rotation
	Vector AxisOfRotation();
	
	// Converts a quaternion to a rotation matrix
	Matrix GetMatrix();
	
	// routine to print it out
	void Print(FILE *outFile);	
	
	bool testMatrixValidity(GLfloat *m);
	}; // class Quaternion

// Scalar left-multiplication
Quaternion operator *(GLfloat scalar, Quaternion quat);

#endif

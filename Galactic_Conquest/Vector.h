
#ifndef VECTOR_H
#define VECTOR_H

// forward declaration for mutually recursive inclusion
class Vector; 

#include <GL/glut.h>
#include <stdio.h>
#include "Point.h"

// generates a single random value in a given range
GLfloat RandomRange(GLfloat min, GLfloat max);

class Vector
	{ // class Vector
	public:
	GLfloat coords[3];

	// default constructor
	Vector();
	
	// copy constructor
	Vector(GLfloat x, GLfloat y, GLfloat z);

	// Copy another Vector & return self
	Vector &operator =(Vector vector);

	// Scalar right-multiplication
	Vector operator * (GLfloat scaleFactor);

	// Scalar right-division
	Vector operator / (GLfloat scaleFactor);

	// Add a point to a vector to get another point 	
	// Does so in Cartesian space, but leaves w-coordinate the same
	Point operator +(Point point);

	// Add a vector to a vector to get another vector 	
	// Does so in Cartesian space, but leaves w-coordinate the same
	Vector operator +(Vector vector);

	// Negation (unary minus)
	Vector operator -();
	
	// Subtraction (returns a vector)
	Vector operator - (Vector vector);

	// array index
	GLfloat &operator [] (int index);
	
	// Computes the length (sqrt of sum of squares)
	GLfloat Length();
	
	// Return the unit (normalized) vector
	Vector Unit();
		
	// Compute the cross-product (this x vector)
	Vector Cross(Vector other);

	// Dot product computes (this . vector)
	GLfloat Dot(Vector other);

	// randomizes vector in a given range of lengths & returns vector
	// note: does NOT do Monte Carlo simulation to get spherical distribution
	static Vector Random(GLfloat min_length, GLfloat max_length);

	// generates a random vector with Monte Carlo simulation
	static Vector RandomAxis();

	// routine to read it in
	// returns 0 on success, non-0 on failure
	long ReadCartesian(FILE *inFile);	
	
	// routine to print it out
	void Print(FILE *outFile);	
	
	void printToConsole();
	}; // class Vector

// scalar left-multiplication
Vector operator *(GLfloat scaleFactor, Vector vector);

#endif

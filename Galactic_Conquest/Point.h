
#ifndef POINT_H
#define POINT_H

// forward declaration for mutually recursive inclusion
class Point;

#include <GL/gl.h>
#include <stdio.h>
#include "Vector.h"

// Assume that we carry EVERYTHING in explicit homogeneous coordinates
class Point
	{ // class Point
	public:
	GLfloat coords[4];
	
	// default constructor
	Point();
	
	// copy constructor
	Point(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	
	// copy constructor (from array of values)
	Point(GLfloat *array, int nValues);
	
	// Copy another Point & return self
	Point &operator = (Point point);
	
	// Scalar right-multiplication
	Point operator * (GLfloat scaleFactor);

	// Scalar right-division
	Point operator / (GLfloat scaleFactor);

	// Add a vector to a point to get another point 	
	// Does so in Cartesian space, but leaves w-coordinate the same
	Point operator +(Vector vector);

	Point Point::operator +(Point point);
	
	// Negation (unary minus)
	Point operator -();
	
	// Subtract a vector from a point
	Point operator -(Vector vector);
	
	// Subtraction (returns a vector)
	Vector operator - (Point point);

	// Divide through by homogeneous coordinate (if a point)
	Point DeHomogenize();
	
	int distanceInTurns(Point p);
	
	float distancex2(Point p);
	
	// routine to read it in as a Cartesian (3 coords)
	// returns 0 on success, non-0 on failure
	long ReadCartesian(FILE *inFile);	
	
	// routines to print it out
	void Print(FILE *outFile);	
	void PrintCartesian(FILE *outFile);	
	void printToConsole();
	}; // class Point

// Scalar left-multiplication
Point operator * (Point point, GLfloat scaleFactor);

#endif

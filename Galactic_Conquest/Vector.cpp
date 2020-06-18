
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "Vector.h"


// default constructor
Vector::Vector()
	{ // constructor
	coords[0] = coords[1] = coords[2] = 0.0f;
	} // constructor
	
// copy constructor
Vector::Vector(GLfloat x, GLfloat y, GLfloat z)
	{ // constructor
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	} // constructor

// Copy another Vector & return self
Vector &Vector::operator =(Vector vector)
	{ // assignment
	for (int i = 0; i < 3; i++)
		coords[i] = vector.coords[i];
	return (*this);
	} // assignment	

// Scalar right-multiplication
Vector Vector::operator * (GLfloat scaleFactor)
	{ // Scalar right-multiplication 
	Vector result;
	for (int i = 0; i < 3; i++)
		result.coords[i] = coords[i] * scaleFactor;
	return result;
	} // Scalar right-multiplication
	
// Scalar right-division
Vector Vector::operator / (GLfloat scaleFactor)
	{ // Scalar right-division 
	Vector result;
	for (int i = 0; i < 3; i++)
		result.coords[i] = coords[i] / scaleFactor;
	return result;
	} // Scalar right-division
	
// scalar left-multiplication
Vector operator *(GLfloat scaleFactor, Vector vector)
	{ // scalar left-multiplication
	Vector result;
	for (int i = 0; i < 3; i++)
		result.coords[i] = scaleFactor * vector.coords[i];
	return result;
	} // scalar left-multiplication

// Computes the length (sqrt of sum of squares)
GLfloat Vector::Length()
	{ // Length()
	return sqrt(Dot(*this));
	} // Length()

// Return the unit (normalized) vector
Vector Vector::Unit()
	{ // Normalize()
	Vector result = (*this) / Length();
	return result;
	} // Normalize()

// Add a vector to a vector to get another vector 	
// Does so in Cartesian space, but leaves w-coordinate the same
Vector Vector::operator +(Vector vector)
	{ // addition
	Vector result;
	for (int i = 0; i < 3; i++)
		result.coords[i] = this->coords[i] + vector.coords[i];
	return result;
	} // addition

// Add a point to a vector to get another point 	
// Does so in Cartesian space, but leaves w-coordinate the same
Point Vector::operator +(Point point)
	{ // addition
	Point result;
	for (int i = 0; i < 3; i++)
		result.coords[i] = this->coords[i]*point.coords[3] + point.coords[i];
	result.coords[3] = point.coords[3];
	return result;
	} // addition

// Negation (unary minus)
Vector Vector::operator -()
	{ // unary minus
	return (*this) * -1;
	} // unary minus
	
// Subtraction (returns a vector)
Vector Vector::operator - (Vector vector)
	{ // minus
	Vector result;
	for (int i = 0; i < 3; i++)
		result.coords[i] = this->coords[i] - vector.coords[i];
	return result;
	} // minus
	
// array index
GLfloat &Vector::operator [] (int index)
	{ // array index
	return coords[index];
	} // array index
	
// Compute the cross-product (this x vector)
Vector Vector::Cross(Vector vector)
	{ // Cross()
	Vector result;
	result.coords[0] = coords[1] * vector.coords[2] - coords[2] * vector.coords[1];
	result.coords[1] = coords[2] * vector.coords[0] - coords[0] * vector.coords[2];
	result.coords[2] = coords[0] * vector.coords[1] - coords[1] * vector.coords[0];
	return result;	
	} // Cross()

// Dot product computes (this . vector)
GLfloat Vector::Dot(Vector vector)
	{ // Dot()
	return (coords[0]*vector.coords[0] + coords[1]*vector.coords[1] + 
		coords[2]*vector.coords[2]);
	} // Dot()

// routine to read it in
// returns 0 on success, non-0 on failure
long Vector::ReadCartesian(FILE *inFile)
	{ // Read()
	// try reading in three values
	long nScanned = fscanf(inFile, "%f %f %f ", 
		&(coords[0]), &(coords[1]), &(coords[2]));

	// check to make sure we read 3 in
	if (nScanned != 3)
		return -1;

	// we succeeded
	return 0;
	} // Read()

// routine to print it out
void Vector::Print(FILE *outFile)
	{ // Print()
	fprintf(outFile, " %10.6f %10.6f %10.6f", 
		coords[0], coords[1], coords[2], coords[3]);
	} // Print()

void Vector::printToConsole(){
	std::cout << "Vector " << coords[0] << " " << coords[1] << " " << coords[2] << std::endl;
}

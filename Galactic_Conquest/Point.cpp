
#include <stdio.h>
#include <math.h>
#include "Point.h"
#include <iostream>

// default constructor
Point::Point()
	{ // constructor
	coords[0] = coords[1] = coords[2] = 0.0f;
	coords[3] = 1.0f;
	} // constructor
	
// copy constructor
Point::Point(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{ // constructor
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	coords[3] = w;
	} // constructor	

// copy constructor
Point::Point(GLfloat *array, int nValues)
	{ // constructor
	// default to something reasonable
	coords[0] = coords[1] = coords[2] = 0.0f;
	coords[3] = 1.0f;

	// then copy the specified number of values
	for (int i = 0; i < (nValues > 4 ? 4 : nValues); i++)
		coords[i] = array[i];
	} // constructor

// Copy another Point & return self
Point &Point::operator = (Point point)
	{ // operator =
	int i;
	for (i = 0; i < 4; i++)
		coords[i] = point.coords[i];
	return (*this);
	} // operator =
	
// Scalar right-multiplication
Point Point::operator * (GLfloat scaleFactor)
	{ // operator *
	Point result;
	int i;
	for (i = 0; i < 4; i++)
		result.coords[i] = coords[i] * scaleFactor;
	return result;
	} // operator *

// Scalar right-division
Point Point::operator / (GLfloat scaleFactor)
	{ // operator /
	Point result;
	int i;
	for (i = 0; i < 4; i++)
		result.coords[i] = coords[i] / scaleFactor;
	return result;
	} // operator /

// Add a vector to a point to get another point 	
// Does so in Cartesian space, but leaves w-coordinate the same
Point Point::operator +(Vector vector)
	{ // operator +
	Point result;
	int i;
	// add the vector, multiplied by w
	for (i = 0; i < 4; i++)
		result.coords[i] = coords[i] + vector.coords[i] * coords[3];
	return result;
	} // operator +

Point Point::operator +(Point point)
	{
	Point result;
	result.coords[0] = coords[0] + point.coords[0];
	result.coords[1] = coords[1] + point.coords[1];
	result.coords[2] = coords[2] + point.coords[2];
	return result;
	}

// Negation (unary minus)
Point Point::operator -()
	{ // operator -
	Point result;
	int i;
	// note that the w coordinate is unchanged
	// either that, or we could flip w instead
	for (i = 0; i < 3; i++)
		result.coords[i] = -coords[i];
	return result;
	} // operator -
	
// Subtract a vector from a point
Point Point::operator -(Vector vector)
	{ // operator -
	Point result;
	int i;
	// subtract the vector, multiplied by w
	for (i = 0; i < 4; i++)
		result.coords[i] = coords[i] - vector.coords[i] * coords[3];
	return result;
	} // operator -
	
// Subtraction (returns a vector)
Vector Point::operator - (Point point)
	{ // operator -
	Vector result;
	int i;
	for (i = 0; i < 3; i++)
		result.coords[i] = coords[i] / coords[3] - point.coords[i] / point.coords[3];
	return result;
	} // operator -

// Divide through by homogeneous coordinate (if a point)
Point Point::DeHomogenize()
	{ // DeHomogenize()
	Point result;
	int i;
	for (i = 0; i < 3; i++)
		result.coords[i] = coords[i] / coords[3];
	coords[3] = 0.0;
	return result;
	} // DeHomogenize()
	
// routine to read it in as a Cartesian (3 coords)
// returns 0 on success, non-0 on failure
long Point::ReadCartesian(FILE *inFile)
	{ // Read()
	// try reading in three values
	long nScanned = fscanf(inFile, "%f %f %f ", 
		&(coords[0]), &(coords[1]), &(coords[2]));

	// check to make sure we read 3 in
	if (nScanned != 3)
		return -1;

	// set homogeneous coordinate
	coords[3] = 1.0;

	// we succeeded
	return 0;
	} // Read()

// routine to print it out
void Point::Print(FILE *outFile)
	{ // Print()
	fprintf(outFile, "(%10.6f, %10.6f, %10.6f, %10.6f)", 
		coords[0], coords[1], coords[2], coords[3]);
	} // Print()

// routine to print it out
void Point::PrintCartesian(FILE *outFile)
	{ // Print()
	fprintf(outFile, " %10.6f %10.6f %10.6f", 
		coords[0]/coords[3], coords[1]/coords[3], coords[2]/coords[3]);
	} // Print()

void Point::printToConsole(){
	std::cout << "Point " << coords[0] << " " << coords[1] << " " << coords[2] << std::endl;
}

// Scalar left-multiplication
Point operator * (Point point, GLfloat scaleFactor)
	{ // operator *
	Point result;
	int i;
	for (i = 0; i < 4; i++)
		result.coords[i] = point.coords[i] * scaleFactor;
	return result;	
	} // operator *

//return distance between points without squaring them
float Point::distancex2(Point p){
	float distancex2 = ((p.coords[0]-coords[0])*(p.coords[0]-coords[0]) +
								(p.coords[1]-coords[1])*(p.coords[1]-coords[1]) +
								(p.coords[2]-coords[2])*(p.coords[2]-coords[2]));
	return distancex2;
}

int Point::distanceInTurns(Point p){
	std::cout << coords[0] << " "
	<< coords[1] << " " 
	<< coords[2] << std::endl;
	
	std::cout << p.coords[0] << " "
	<< p.coords[1] << " " 
	<< p.coords[2] << std::endl;
	GLfloat distance = sqrt((p.coords[0]-coords[0])*(p.coords[0]-coords[0]) +
							(p.coords[1]-coords[1])*(p.coords[1]-coords[1]) +
							(p.coords[2]-coords[2])*(p.coords[2]-coords[2]));
	std::cout << "distance is " << distance << std::endl;
	int turns = (int)distance/7;
	return turns;
}


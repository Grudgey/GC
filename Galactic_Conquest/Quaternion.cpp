
#include <math.h>
#include <cmath>
#include <iostream>
#include "Quaternion.h"

using namespace std;

#ifndef PI
#define PI 3.14159265358979f
#endif

// constructor
Quaternion::Quaternion()
	{ // constructor
	coords[0] = coords[1] = coords[2] = 0.0;
	coords[3] = 1.0;
	} // constructor

// constructor: sets the quaternion to (x, y, z, w)
Quaternion::Quaternion(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{ // constructor
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	coords[3] = w;
	} // constructor
// Set to a pure scalar value
Quaternion::Quaternion(double scalar)
	{ // copy scalar
	// set first three coords to 0.0
	for (int i = 0; i < 3; i++)
		coords[i] = 0.0;
	// and the real part to the scalar
	coords[3] = scalar;
	} // copy scalar

// Set to a pure vector value
Quaternion::Quaternion(Vector vector)
	{ // copy vector
	// copy vector part
	for (int i = 0; i < 3; i++)
		coords[i] = vector.coords[i];
	// set the real part to 0.0
	coords[3] = 0.0;
	} // copy vector
	
// Set to a homogeneous point
Quaternion::Quaternion(Point point)
	{ // copy point
	// just copy the coordinates
	for (int i = 0; i < 4; i++)
		coords[i] = point.coords[i];
	} // copy point
	
// Set to a rotation defined by a rotation matrix
// WARNING: MATRIX MUST BE A VALID ROTATION MATRIX
Quaternion::Quaternion(Matrix matrix)
	{ // copy rotation matrix
		coords[3] = sqrt(matrix.coords[0] +  matrix.coords[4] + matrix.coords[8] + 1)/2;
		coords[0] = (matrix.coords[5] - matrix.coords[7])/2*coords[3];
		coords[1] = (matrix.coords[6] - matrix.coords[2])/2*coords[3];
		coords[2] = (matrix.coords[1] - matrix.coords[3])/2*coords[3];

	} // copy rotation matrix
	
// Set to a rotation defined by an axis and angle
void Quaternion::SetRotation(Vector axis, GLfloat theta)
	{ // SetRotation()
	
//	        x   =   ax * sin(theta/2)
//        y   =   ay * sin(theta/2)
//        z   =   az * sin(theta/2)
	GLfloat angle = sin(theta/2);
	this->coords[0] = axis.coords[0]*angle;
	this->coords[1] = axis.coords[1]*angle;
	this->coords[2] = axis.coords[2]*angle;
	this->coords[3] = cos(theta*2);

	} // SetRotation()

// Copy another Quaternion & return self
Quaternion &Quaternion::operator = (Quaternion other)
	{ // copy quaternion
	for (int i = 0; i < 4; i++)
		coords[i] = other.coords[i];
	return (*this);	
	} // copy quaternion

// Computes the norm (sum of squares)
GLfloat Quaternion::Norm()
	{ // Norm()
	return (coords[0]*coords[0]+coords[1]*coords[1]+
		coords[2]*coords[2]+coords[3]*coords[3]);
	} // Norm()
	
// Reduce to unit quaternion
Quaternion Quaternion::Unit()
	{ // Unit()
	Quaternion result;
	// get the square root of the norm
	GLfloat sqrtNorm = Norm();
	// now divide by it
	for (int i = 0; i < 4; i++)
		result.coords[i] = coords[i] / sqrtNorm;
	return result;
	} // Unit()
	
// Conjugate the quaternion
Quaternion Quaternion::Conjugate()
	{ // Conjugate()
	Quaternion result;
	for (int i = 0; i < 3; i++)
		result.coords[i] = coords[i] * -1;
	result.coords[3] = coords[3];
	return result;
	} // Conjugate()
	
// Invert the quaternion
Quaternion Quaternion::Inverse()
	{ // Invert()
	Quaternion result = Conjugate() / Norm();
	return result;
	} // Invert()

// Scalar left-multiplication
Quaternion operator *(GLfloat scalar, Quaternion quat)
	{ // scalar left-multiplication
	Quaternion result;
	for (int i = 0; i < 4; i++)
		result.coords[i] = scalar * quat.coords[i];
	return result;
	} // scalar left-multiplication

// Scalar right-multiplication
Quaternion Quaternion::operator *(GLfloat scalar)
	{ // scalar right-multiplication
	Quaternion result;
	for (int i = 0; i < 4; i++)
		result.coords[i] = coords[i] * scalar;
	return result;
	} // scalar right-multiplication
	
// Scalar right-division
Quaternion Quaternion::operator /(GLfloat scalar)
	{ // scalar right-division
	Quaternion result;
	for (int i = 0; i < 4; i++)
		result.coords[i] = coords[i] / scalar;
	return result;
	} // scalar right-division

// Adds two quaternions together
Quaternion Quaternion::operator +(Quaternion other)
	{ // addition
	Quaternion result;
	for (int i = 0; i < 4; i++)
		result.coords[i] = coords[i] + other.coords[i];
	return result;
	} // addition

// Subtracts one quaternion from another
Quaternion Quaternion::operator -(Quaternion other)
	{ // subtraction
	Quaternion result;
	for (int i = 0; i < 4; i++)
		result.coords[i] = coords[i] - other.coords[i];
	return result;
	} // subtraction

// Multiplies two quaternions together
Quaternion Quaternion::operator *(Quaternion other)
	{ // multiplication
	Quaternion newQuaternion;

	newQuaternion.coords[0] = this->coords[3]*other.coords[0] + 
								this->coords[0]*other.coords[3] + 
								this->coords[1]*other.coords[2] -
								this->coords[2]*other.coords[1];
								
	newQuaternion.coords[1] = this->coords[3]*other.coords[1] - 
								this->coords[0]*other.coords[2] + 
								this->coords[1]*other.coords[3] +
								this->coords[2]*other.coords[0];
								
	newQuaternion.coords[2] = this->coords[3]*other.coords[2] + 
								this->coords[0]*other.coords[1] - 
								this->coords[1]*other.coords[0] +
								this->coords[2]*other.coords[3];
								
	newQuaternion.coords[3] = this->coords[3]*other.coords[3] - 
								this->coords[0]*other.coords[0] - 
								this->coords[1]*other.coords[1] -
								this->coords[2]*other.coords[2];
								
	return newQuaternion;

	} // multiplication

// Acts on a vector
Vector Quaternion::Act(Vector vector)
	{ // Act()
	Quaternion tempQuaternion(this->coords[0],this->coords[1],this->coords[2],this->coords[3]);
	Quaternion tempVector(vector.coords[0],vector.coords[1],vector.coords[2],0);
	tempVector = tempQuaternion*tempVector*tempQuaternion.Inverse();
	return Vector(tempVector.coords[0],tempVector.coords[1],tempVector.coords[2]);

	} // Act()
	
// Acts on a homogeneous point
Point Quaternion::Act(Point point)
	{ // Act()
	Quaternion tempQuaternion(this->coords[0],this->coords[1],this->coords[2],this->coords[3]);
	Quaternion tempPoint(point.coords[0],point.coords[1],point.coords[2],0);
	tempPoint = tempQuaternion*tempPoint*tempQuaternion.Inverse();
	return Point(tempPoint.coords[0],tempPoint.coords[1],tempPoint.coords[2],1.0f);
	
	} // Act()
	
// Returns the angle 2*theta of the action in degrees
GLfloat Quaternion::AngleOfAction()
	{ // AngleOfAction()

	return 2*acos(this->coords[3]);

	} // AngleOfAction()
	
// Returns the axis of rotation
Vector Quaternion::AxisOfRotation()
	{ // AxisOfRotation()
	GLfloat angle = sin(this->AngleOfAction());
	GLfloat x = this->coords[0]/angle;
	GLfloat y = this->coords[1]/angle;
	GLfloat z = this->coords[2]/angle;
	
	return Vector(x,y,z);

	} // AxisOfRotation()

// routine to print it out
void Quaternion::Print(FILE *outFile)
	{ // Print()
	printf("(%8.5f, %8.5f, %8.5f, %8.5f)", coords[0], coords[1], 
		coords[2], coords[3]);
	} // Print()

// Converts a quaternion to a rotation matrix
Matrix Quaternion::GetMatrix()
	{ // GetMatrix()
	
	Matrix newMatrix;
	GLfloat x = this->coords[0];
	GLfloat y = this->coords[1];
	GLfloat z = this->coords[2];
	GLfloat w = this->coords[3];
	
	newMatrix.coords[0] = 1 - 2 * ( y*y + z*z );
	newMatrix.coords[1] = 2 * ( x*y - z*w );
	newMatrix.coords[2] = 2 * ( x*z + y*w );
	newMatrix.coords[3] = 0;
	newMatrix.coords[4] = 2 * ( x*y + z*w );
	newMatrix.coords[5] = 1 - 2 * ( x*x + z*z );
	newMatrix.coords[6] = 2 * ( y*z - x*w );
	newMatrix.coords[7] = 0;
	newMatrix.coords[8] = 2 * ( x*z - y*w );
	newMatrix.coords[9] = 2 * ( y*z + x*w );
	newMatrix.coords[10] = 1 - 2 * ( x*x + y*y );
	newMatrix.coords[11] = 0;
	newMatrix.coords[12] = 0;
	newMatrix.coords[13] = 0;
	newMatrix.coords[14] = 0;
	newMatrix.coords[15] = 1;
	
	// this code returns an identity matrix so that rendering works:
	// you will need to replace it
	return newMatrix;

	} // GetMatrix()

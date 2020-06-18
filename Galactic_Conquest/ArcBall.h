
#include <GL/glut.h>
#include "Quaternion.h"

#ifndef ARCBALL_H
#define ARCBALL_H

class ArcBall
	{ // class ArcBall
	public:
	Quaternion u;
	Quaternion v;
	Quaternion startingRotation;
	GLfloat lastx;
	GLfloat lasty;
	// the base rotation
	Quaternion lastRotation;
	// the current rotation
	Quaternion currentRotation;
	
	// constructor - initializes to a zero rotation
	ArcBall();

	// convert an (x,y) point to a quaternion
	Quaternion FindQuat(GLfloat x, GLfloat y);

	// start the dragging process with a point in the range [-1..1]^2
	void StartDrag(GLfloat x, GLfloat y);
	
	// continue the dragging process with another such point
	void UpdateDrag(GLfloat x, GLfloat y);
	
	// stop dragging
	void EndDrag(GLfloat x, GLfloat y);
	
	// extract the rotation matrix for rendering purposes
	Matrix GetRotation();
	
	}; // class ArcBall

#endif

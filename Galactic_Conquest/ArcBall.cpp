#include "ArcBall.h"
#include "GameVariables.h"
#include <math.h>
#include <iostream>
#include <stdio.h>

using namespace std;

// constructor - initializes to a zero rotation
ArcBall::ArcBall()
	{ // constructor
	lastRotation.coords[0] = lastRotation.coords[1] = lastRotation.coords[2] = 0;
	lastRotation.coords[3] = 1;
	
	currentRotation.coords[0] = currentRotation.coords[1] = currentRotation.coords[2] = 0;
	currentRotation.coords[3] = 1;
	} // constructor


// start the dragging process with a point in the range [-1..1]^2
void ArcBall::StartDrag(GLfloat x, GLfloat y)
	{ // StartDrag()

	GLfloat z = 0.0;
	GLfloat length = x*x + y*y;
    if (length > 1.0f)
    {
        GLfloat norm;

        //Compute a normalizing factor (radius / sqrt(length))
        norm    = 1.0f / sqrt(length);

        //Return the "normalized" vector, a point on the sphere
        x = x * norm;
        y = y * norm;
        z = 0.0f;
    }
    else    //Else it's on the inside
    {
        //Return a vector to a point mapped inside the sphere sqrt(radius squared - length)

        z = sqrt(1.0f - length);
    }
    u.coords[0] = x;
    u.coords[1] = y;
    u.coords[2] = z;
    u.coords[3] = 0.0f;
    startingRotation = currentRotation;
	} // StartDrag()
	
// continue the dragging process with another such point
void ArcBall::UpdateDrag(GLfloat x, GLfloat y)
	{ // UpdateDrag()
	lastx = x;
	lasty = y;
	GLfloat z = 0.0;
	GLfloat length = x*x + y*y;
    if (length > 1.0f)
    {
        GLfloat norm;
        norm    = 1.0f / sqrt(length);
        x = x * norm;
        y = y * norm;
        z = 0.0f;
    }
    else
    {
        z = sqrt(1.0f - length);
    }
    v.coords[0] = x;
    v.coords[1] = y;
    v.coords[2] = z;
    v.coords[3] = 0.0f;
    lastRotation = v*u.Inverse();
    if(currentRotation.Norm()-1 > 0.0001){
    	currentRotation.Unit();
    	cout << "Normalising quaternion" << endl;
    }
	currentRotation = lastRotation*startingRotation;
	} // UpdateDrag()
	
// stop dragging
void ArcBall::EndDrag(GLfloat x, GLfloat y)
	{ // EndDrag()
	if(x != lastx || lasty != y){
	GLfloat z = 0.0;
	GLfloat length = x*x + y*y;

    //If the point is mapped outside of the sphere... (length > radius squared)
    if (length > 1.0f)
    {
        GLfloat norm;

        //Compute a normalizing factor (radius / sqrt(length))
        norm    = 1.0f / sqrt(length);

        //Return the "normalized" vector, a point on the sphere
        x = x * norm;
        y = y * norm;
        z = 0.0f;
    }
    else    //Else it's on the inside
    {
        //Return a vector to a point mapped inside the sphere sqrt(radius squared - length)

        z = sqrt(1.0f - length);
    }
    v.coords[0] = x;
    v.coords[1] = y;
    v.coords[2] = z;
    v.coords[3] = 0.0f;
    lastRotation = v*u.Inverse();
	currentRotation = lastRotation*startingRotation;
	}
	} // EndDrag()
	

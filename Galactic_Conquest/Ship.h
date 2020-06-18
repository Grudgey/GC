#ifndef SHIP_H_
#define SHIP_H_
#include <GL/gl.h>
#include "Model.h"
#include "Particle.h"
#include "Point.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Starsystem.h"

class Ship
{
public:
	Ship(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat x, GLfloat y, GLfloat z,
			int unique_r, int unique_g,
					int unique_b,Starsystem *systemDestination,Model *pModel);
	virtual ~Ship();
	void drawShip(bool active);
	void drawUniqueShip();
	void drawTrajectory();
	void updateSystemDestination(Starsystem *s);
	void updateCurrentSystem(Starsystem *s);
	void leaveCurrentSystem();
	void startMovementAnimation();
	void updateOrientation();
	Point getCoordinates();
	void setCoordinates(Point p);
	Matrix interpolateOrientation();
	Point interpolateMovement();
	bool doneRotating;
	bool doneMoving;
	bool checkForCollision(Point otherCoordinates, GLfloat *otherBoundingSpheresRadii, Point *otherBoundingSpheresCentres);
	GLfloat x,y,z;
	int uniqueColour[3];
	//-0.455, 2.0, -2.383
	Point boundingSpheresCentres[3];
	//1.11.7, 4.325  , -3.837
	//[0] 1.565, [1] 2.325, [2] 1.454
	GLfloat boundingSpheresRadii[3];
	int arrivalTurns;
	bool hasArrived;
	Vector moveVector;
	bool moving;
	Point destinationCoords;
	Starsystem *systemDestination;
	Starsystem *currentSystem;
	Model *pModel;
	Matrix orientation;
	Quaternion previousOrientation;
	Quaternion currentOrientation;
	float distance;
	float acceleration;
	float velocity;
	float rotation;
	float angularVelocity;
	float angularAcceleration;
	Vector u;
	Vector v;
	static const int particleNumber = 500;
	Particle rightJet[particleNumber];
	Particle leftJet[particleNumber];
	Particle mainJet[particleNumber];
private:
	bool enroute;
	Point coordinates;
	Point previousCoordinates;
	Point tempCoordinates;
	GLfloat colour[4];
	int fleet;
};

#endif /*SHIP_H_*/

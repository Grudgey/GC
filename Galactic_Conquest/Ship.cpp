#include "Ship.h"
#include "GL/glut.h"
#include "Vector.h"
#include <cmath>
#include <iostream>

GLfloat lightGreen[4] = { 0.0f,1.0f,0.0f,0.6f };
GLfloat lightBlue[4] = { 0.0f,0.0f,1.0f,0.4f };
GLfloat black[4] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat red[4] = { 1.0f,0.1f,0.0f,1.0f };
int trajectoryFrame = 0;
int blinkTurn = 0;
bool blink = true;

Ship::Ship(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat x, GLfloat y, GLfloat z,
		int unique_r, int unique_g, int unique_b,Starsystem *systemDestination,Model *pModel)

{
	colour[0] = r;
	colour[1] = g;
	colour[2] = b;
	colour[3] = a;
	this->x = x;
	this->y = y;
	this->z = z;
	coordinates.coords[0] = destinationCoords.coords[0] = x;
	coordinates.coords[1] = destinationCoords.coords[1] = y;
	coordinates.coords[2] = destinationCoords.coords[2] = z;
	//-0.455, 2.0 , -2.383
	//[0] 1.565, [1] 2.325, [2] 1.454
	boundingSpheresCentres[0].coords[2] = -0.455;
	boundingSpheresCentres[1].coords[2] = 2;
	boundingSpheresCentres[2].coords[2] = -2.383;
	boundingSpheresRadii[0] = 1.565;
	boundingSpheresRadii[1] = 2.325;
	boundingSpheresRadii[2] = 1.454;
	previousCoordinates = coordinates;
	uniqueColour[0] = unique_r;
	uniqueColour[1] = unique_g;
	uniqueColour[2] = unique_b;
	arrivalTurns = 0;
	doneMoving = doneRotating = true;
	hasArrived = true;
	moving = false;
	Quaternion q = Quaternion(0,0,0,1);
	q = previousOrientation = currentOrientation;
	orientation = q.GetMatrix();
	distance = rotation = 1.01f;
	this->pModel = pModel;
	this->systemDestination = this->currentSystem = systemDestination;
	u = Vector(0,0,-1);
	u.printToConsole();
	
		for(int i = 0; i < particleNumber; i++){
			rightJet[i] = Particle(Point(0.473f,0.247f,2.99f,1.0f), Vector(0,0,0.02),60,colour,0.2);
			leftJet[i] = Particle(Point(-0.520f,0.247f,2.99f,1.0f), Vector(0,0,0.02),60,colour,0.2);
			mainJet[i] = Particle(Point(0.0f,0.647f,3.226f,1.0f), Vector(0,0,0.03),60,colour,0.2);
		}
	}

Ship::~Ship()
{
}

void Ship::updateSystemDestination(Starsystem *s){
	if(s != systemDestination){
	systemDestination = s;
	updateOrientation();
	}
}

void Ship::updateCurrentSystem(Starsystem *s){
	if(s != currentSystem){
		currentSystem = s;
	}
}

void Ship::leaveCurrentSystem(){
	if(!moving){
		std::cout << "Leaving system " << std::endl;
		hasArrived = false;
		moving = true;
		currentSystem->homeShips--;
		Vector u = systemDestination->coordinates - this->coordinates;
	}
}

void Ship::startMovementAnimation(){
	doneMoving = false;
	distance = 0.0f;
	velocity = 0.0f;
	acceleration = 0.0005;
}

void Ship::drawShip(bool active){
	glMatrixMode(GL_MODELVIEW);
		if(!active){
			glDisable(GL_COLOR_MATERIAL);
		}
		else{
			drawTrajectory();
			glColor3f(1.0f,1.0f,1.0f);
		}
			glPushMatrix();
			Point p = interpolateMovement();
			glTranslatef(p.coords[0],p.coords[1],p.coords[2]);
			glMultMatrixf(interpolateOrientation().coords);
			
			pModel->draw();
			glEnable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, red);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glEnable(GL_BLEND);
			glPointSize(5);
				if(distance < 1.0f)
				{
					
					for(int i = 0; i < particleNumber; i++)
					{
						rightJet[i].drawParticle(true);
						leftJet[i].drawParticle(true);
						mainJet[i].drawParticle(true);
					}
				}
				else 
				{
					for(int i = 0; i < particleNumber-300; i++)
					{
						rightJet[i].drawParticle(false);
						leftJet[i].drawParticle(false);
						mainJet[i].drawParticle(false);
					}
				}
				glDisable(GL_BLEND);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
			
			glPopMatrix();
				
			
}

void Ship::drawUniqueShip(){
		glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			Point p = interpolateMovement();
			glTranslatef(p.coords[0],p.coords[1],p.coords[2]);
			glMultMatrixf(interpolateOrientation().coords);
			glColor3ub(uniqueColour[0],uniqueColour[1],uniqueColour[2]);
			pModel->draw();
			glPopMatrix();
}

void Ship::drawTrajectory()
{
	if(destinationCoords.coords[0] != coordinates.coords[0] &&
			destinationCoords.coords[1] != coordinates.coords[1] &&
			destinationCoords.coords[2] != coordinates.coords[2] && 
			doneRotating && doneMoving && currentSystem != systemDestination)
	{
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glEnable(GL_BLEND);
		glPushMatrix();
			glTranslatef(0.0f,0.5f,0.0f);
			glColor4f(colour[0], colour[1], colour[2], 0.6f);
			glEnable(GL_LINE_SMOOTH);
			glBegin(GL_QUADS);
				glVertex3f(systemDestination->coordinates.coords[0]-0.2,
						systemDestination->coordinates.coords[1],
						systemDestination->coordinates.coords[2]);
				glVertex3f(systemDestination->coordinates.coords[0]+0.2,
						systemDestination->coordinates.coords[1],
						systemDestination->coordinates.coords[2]);
				glVertex3f(this->coordinates.coords[0]+0.2,
						this->coordinates.coords[1],
						this->coordinates.coords[2]);
				glVertex3f(this->coordinates.coords[0]-0.2,
						this->coordinates.coords[1],
						this->coordinates.coords[2]);
				
				glVertex3f(this->coordinates.coords[0]-0.2,
						this->coordinates.coords[1],
						this->coordinates.coords[2]);
				glVertex3f(this->coordinates.coords[0]+0.2,
						this->coordinates.coords[1],
						this->coordinates.coords[2]);
				glVertex3f(systemDestination->coordinates.coords[0]+0.2,
						systemDestination->coordinates.coords[1],
						systemDestination->coordinates.coords[2]);
				glVertex3f(systemDestination->coordinates.coords[0]-0.2,
						systemDestination->coordinates.coords[1],
						systemDestination->coordinates.coords[2]);
			glEnd();
			
		if(blink)
		{	
			glLineWidth(8);
			glBegin(GL_LINES);
				Point currentPoint = moveVector*(blinkTurn) + coordinates;
				Point nextPoint = moveVector*(blinkTurn+1) + coordinates;
				glVertex3f(currentPoint.coords[0], currentPoint.coords[1], currentPoint.coords[2]);
				glVertex3f(nextPoint.coords[0], nextPoint.coords[1], nextPoint.coords[2]);
			glEnd();
			glLineWidth(1);
		}
			glDisable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
		glPopMatrix();
		
		trajectoryFrame++;
		if(trajectoryFrame >= 30)
		{
			trajectoryFrame = 0;
			blink = !blink;
			if(blink)
			{
				blinkTurn++;
			}
			if(blinkTurn > arrivalTurns-1){
				blinkTurn = 0;
			}
		}
	}
}

Point Ship::getCoordinates(){
	return coordinates;
}

void Ship::setCoordinates(Point p){
	coordinates = p;
}

void Ship::updateOrientation(){
	std::cout << "Not done animating" << std::endl;
	doneRotating = false;
	v = systemDestination->coordinates - this->coordinates;
	u = u.Unit();
	v = v.Unit();
	Vector uv = u+v;
	uv = uv.Unit();
	Vector normal = uv.Cross(v);
	currentOrientation.coords[0] = normal[0]; 
	currentOrientation.coords[1] = normal[1];
	currentOrientation.coords[2] = normal[2];
	currentOrientation.coords[3] = uv.Dot(v);
	currentOrientation = currentOrientation.Inverse();
	angularVelocity = 0.0f;
	rotation = 0.0f;
	angularAcceleration = 0.0005;
}

bool Ship::checkForCollision(Point otherCoordinates, GLfloat *otherBoundingSpheresRadii, Point *otherBoundingSpheresCentres){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){

			Point thisCentre = boundingSpheresCentres[i]+coordinates;
			Point otherCentre = boundingSpheresCentres[j]+otherCoordinates;
			
			float distancex2 = thisCentre.distancex2(otherCentre);
			float radiiSumx2 = (boundingSpheresRadii[i] + boundingSpheresRadii[j])*
							(boundingSpheresRadii[i] + boundingSpheresRadii[j]);
			if(distancex2 < radiiSumx2){
				return true;
			}
		}
	}
	return false;
}

Point Ship::interpolateMovement(){
	if(distance <= 1.0){
		tempCoordinates.coords[0] = previousCoordinates.coords[0] + distance*(coordinates.coords[0] - previousCoordinates.coords[0]);
		tempCoordinates.coords[1] = previousCoordinates.coords[1] + distance*(coordinates.coords[1] - previousCoordinates.coords[1]);
		tempCoordinates.coords[2] = previousCoordinates.coords[2] + distance*(coordinates.coords[2] - previousCoordinates.coords[2]);
		distance += velocity;
		velocity += acceleration;
			if(distance > 0.5){
				acceleration = -0.0005;
			}
			if(distance > 1.0f){
				previousCoordinates = coordinates;
			}
			
			return tempCoordinates;
	}
	else {
		if(!doneMoving){
			doneMoving = true;
		}
		return this->coordinates;
	}
}

//interpolate between orientations using spherical linear interpolation
Matrix Ship::interpolateOrientation(){
	if(rotation <= 1.0){
	float cosThetaDiv2 = previousOrientation.coords[3]*currentOrientation.coords[3] + 
						previousOrientation.coords[0]*currentOrientation.coords[0] + 
						previousOrientation.coords[1]*currentOrientation.coords[1] + 
						previousOrientation.coords[2]*currentOrientation.coords[2];
	float thetaDiv2 = acosf(cosThetaDiv2);
	Quaternion interpolatedOrientation = (previousOrientation*sinf((1-rotation)*thetaDiv2) + currentOrientation*sinf(rotation*thetaDiv2))/sinf(thetaDiv2);
	orientation = interpolatedOrientation.GetMatrix();
	rotation += angularVelocity;
	angularVelocity += angularAcceleration;
	
		if(rotation > 0.5){
			angularAcceleration = -0.0005;
		}
		if(rotation > 1.0f){
			std::cout << "Updating orientation" << std::endl;
			previousOrientation = currentOrientation;
		}
		
	return orientation;
	
	}
	else {
		if(!doneRotating){
			doneRotating = true;
		}
		return orientation;
	}
}


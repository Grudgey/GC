#include "Particle.h"
#include <stdlib.h>
#include <GL/glext.h>
#include <iostream>

Particle::Particle(Point p, Vector velocity, int lifeTime, GLfloat *colour, GLfloat radius)
{
	GLfloat r2 = radius*radius;
	float d;
	int lifeTimeOffSet;
	//where to draw from z shouldnt change, x and y should vary slightly
	bool withinRadius = false;
	bool engineThrust = false;
		
	while(!withinRadius)
	{	
		int radiusInt = (int)(radius*1000);
		GLfloat xOffset = ((GLfloat)(rand()%(radiusInt*2))-radiusInt)/1000;
		GLfloat yOffset = ((GLfloat)(rand()%(radiusInt*2))-radiusInt)/1000;
		this->p.coords[0] = p.coords[0] + xOffset;
		this->p.coords[1] = p.coords[1] + yOffset;
		this->p.coords[2] = p.coords[2];
		d = this->p.distancex2(p);
		lifeTimeOffSet = (int)(d*500);
		if(d < r2){
			withinRadius = true;
		}
	}
	//vector used to displace coordinates each frame, shud be random within a certain radius
	//closer to centre means less random
	
	this->velocity = velocity;
	this->velocity.coords[2] += -d/1.5;
	currentVelocity = this->velocity;
	//closer to centre means longer life
	if(lifeTimeOffSet == 0) lifeTimeOffSet = 1;
	this->lifeTime = currentLifeTime = lifeTime-(rand()%lifeTimeOffSet);
	this->colour[0] = 1.0;
	this->colour[1] = 0.2f+d*20;
	this->colour[2] = 0.0f;
	this->colour[3] = 1.0f;
	this->radius = radius;
	this->currentP = this->p;
}

Particle::Particle(){
	
}

Particle::~Particle()
{
}

void Particle::drawParticle(bool shipMovementAnimation){
	glPushMatrix();
	glScalef(1.0f,1.0f,0.5f);
	glTranslatef(0.0f,0.0f,3.0f);
	glColor4fv(colour);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	glVertex3f(currentP.coords[0],currentP.coords[1],currentP.coords[2]);
	glEnd();
	glPopMatrix();
	currentP = currentVelocity + currentP;
	currentLifeTime--;
	if(currentLifeTime == 0){
		restartParticle(shipMovementAnimation);
	}
}

void Particle::restartParticle(bool shipMovementAnimation){
	currentLifeTime = lifeTime;
	currentP = p;
	currentVelocity = velocity;
	if(shipMovementAnimation){
		currentVelocity.coords[2] += 0.1;
	}
}



#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "GL/gl.h"
#include "GL/glu.h"
#include "Vector.h"

class Particle
{
public:
	Particle();
	Particle(Point p, Vector velocity, int lifeTime, GLfloat *colour, GLfloat radius);
	virtual ~Particle();
	Point p;
	Point currentP;
	Vector velocity;
	Vector currentVelocity;
	Vector thrustvelocity;
	int lifeTime;
	int thrustlifeTime;
	int currentLifeTime;
	GLfloat colour[4];
	GLfloat radius;
	void drawParticle(bool shipMovementAnimation);
	void restartParticle(bool shipMovementAnimation);
};

#endif /*PARTICLE_H_*/

#ifndef STARSYSTEM_H_
#define STARSYSTEM_H_
#include <vector>
#include <GL/gl.h>
#include "Model.h"
#include "Point.h"
#include <vector>
#include <GL/glu.h>
#include <GL/glut.h>

class Starsystem
{
public:
	Starsystem(int team, GLfloat x, GLfloat y, GLfloat z,float radius,
			int unique_r, int unique_g,
					int unique_b, Model *model, Model *model_blue, Model *model_green);
	Starsystem();
	virtual ~Starsystem();
	void drawStarsystem(bool selected);
	void drawUniqueStarsystem();
	void drawHalo();
	int team;
	bool inConflict;
	float radius;
	int homeShips;
	int enemyShips;
	Model *starsystem_model;
	Model *starsystem_model_green;
	Model *starsystem_model_blue;
	int uniqueColour[3];
	Point coordinates;
	GLfloat x,y,z;
};

#endif /*STARSYSTEM_H_*/

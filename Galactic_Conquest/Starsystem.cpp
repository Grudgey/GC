#include "Starsystem.h"
#include <iostream>
#include <cmath>

using namespace std;

#define PID2 3.141592654
#define TWOPI 6.283185308

static GLUquadricObj *q = gluNewQuadric();
GLfloat white[4] = { 1.0f, 1.0f, 1.0f, 0.2f };
GLfloat blue[4] = { 0.0f, 0.0f, 1.0f, 0.2f };
GLfloat green[4] = { 0.2f, 0.8f, 0.0f, 0.1f };

Starsystem::Starsystem(int team, GLfloat x, GLfloat y, GLfloat z,float radius,
		int unique_r, int unique_g,
				int unique_b, Model *model, Model *model_blue, Model *model_green)
{
	this->x = x;
	this->y = y;
	this->z = z;
	gluQuadricNormals(q, GL_SMOOTH);
	coordinates.coords[0] = x;
	coordinates.coords[1] = y;
	coordinates.coords[2] = z;
	starsystem_model = model;
	starsystem_model_blue = model_blue;
	starsystem_model_green = model_green;
	this->team = team;
	this->radius = radius;
	enemyShips = homeShips = 0;
	uniqueColour[0] = unique_r;
	uniqueColour[1] = unique_g;
	uniqueColour[2] = unique_b;
	inConflict = false;
}

Starsystem::Starsystem()
{
}

Starsystem::~Starsystem()
{
}

void Starsystem::drawStarsystem(bool selected){
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(coordinates.coords[0],coordinates.coords[1],coordinates.coords[2]);
		if(team == 0) {
			glColor4fv(white);
			glDisable(GL_COLOR_MATERIAL);
			starsystem_model->draw();
			glEnable(GL_COLOR_MATERIAL);
		}
		else if(team == 1) {
			glColor4fv(blue);
			glDisable(GL_COLOR_MATERIAL);
			starsystem_model_blue->draw();
			glEnable(GL_COLOR_MATERIAL);
		}
		else if(team == 2) {
			glColor4fv(green);
			glDisable(GL_COLOR_MATERIAL);
			starsystem_model_green->draw();
			glEnable(GL_COLOR_MATERIAL);
		}
	    glPopMatrix();
		   	if(selected)
		   	{
		   		drawHalo();
		   	}
}

void Starsystem::drawHalo(){
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_COLOR_MATERIAL);
	glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glEnable(GL_BLEND);
		glTranslatef(coordinates.coords[0],coordinates.coords[1],coordinates.coords[2]);
		glScalef(1.2,1.2,1.2);
		gluQuadricTexture(q,false);
		gluQuadricNormals(q, GL_SMOOTH);
		gluSphere(q,radius,20,14);
	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
}

void Starsystem::drawUniqueStarsystem(){
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glColor3ub(uniqueColour[0],uniqueColour[1],uniqueColour[2]);
	    glTranslatef(x,y,z);
	    glutSolidSphere(3,6,6);
	    gluSphere(q,radius,20,14);
		glPopMatrix();
}

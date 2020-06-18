#ifndef RENDER_H_
#define RENDER_H_
#include "GL/gl.h"
#include "SDL/SDL.h"
#include "GameVariables.h"
#include "Vector.h"
#include "Point.h"

class Render
{
public:
	Render();
	virtual ~Render();
	void reshape(int width, int height);
	void init();
	void redisplay(SDL_Surface *drawContext, GLfloat *modelView, GLfloat current_zoom);
	void yaw(float angle);
	void pitch(float angle);
	void zoom(GLfloat zoom);
	void roll(float angle);
	void loadTexture(char *string);
	void setViewMatrix(float x, float y, float z);
	void updateViewMatrix(GLfloat *modelView);
	void invertMatrix(GLfloat *m);
	float viewMatrix[16];
	float worldMatrix[16];
	
public:
	GLfloat ambientLight[4];
	GLfloat diffuseLight[4];
	GLfloat specularLight[4];
	GLfloat position[4];
	GLfloat xmove;
	GLfloat ymove;
	GLfloat zmove;
	GLfloat anglex;
	GLfloat angley;
	GLfloat anglez;
	GLfloat rotx;
	GLfloat roty;
	GLfloat rotz;
};

#endif /*RENDER_H_*/

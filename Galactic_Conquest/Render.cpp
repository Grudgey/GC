#include "Render.h"
#include "GL/glu.h"
#include <cmath>
#include <iostream>

using namespace std;
Render::Render()
{

}

Render::~Render()
{
}

void Render::reshape(int width, int height){
	  GLfloat h = (GLfloat) height / (GLfloat) width;
	  glViewport(0, 0, (GLint) width, (GLint) height);
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
	  glMatrixMode(GL_MODELVIEW);
	  glLoadIdentity();
	  glTranslatef(0.0, 0.0, 0.0);
}

void Render::init(){
	//ambient light colour
	ambientLight[0] = 0.4f;
	ambientLight[1] = 0.4f;
	ambientLight[2] = 0.4f;
	ambientLight[3] = 1.0f;
	
	//diffuse light colour
	diffuseLight[0] = 0.4f;
	diffuseLight[1] = 0.4f;
	diffuseLight[2] = 0.4f;
	diffuseLight[3] = 1.0f;
	
	//specular light colour
	specularLight[0] = 0.7f;
	specularLight[1] = 0.7f;
	specularLight[2] = 0.7f;
	specularLight[3] = 1.0f;
	
	//light direction
	position[0] = -1.5f;
	position[1] = 1.0f;
	position[2] = -4.0f;
	position[3] = 1.0f;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_SHADE_MODEL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void Render::redisplay(SDL_Surface *drawContext, GLfloat *modelView, GLfloat current_zoom){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0, 0.0f, 0.0f, 0.0f);// Clear The Screen And The Depth Buffer
	reshape(drawContext->w, drawContext->h);
	windowHeight = drawContext->h;
	windowWidth = drawContext->w;
	zoom(current_zoom);
	updateViewMatrix(modelView);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
}

//setting up the view matrix at the origin with translation x y z (view position)
void Render::setViewMatrix(float x, float y, float z) {
	// ( 1  0  0  x)
	// ( 0  1  0  y)
	// (-0 -0 -1 -z)
	// ( 0  0  0  1)
	
	viewMatrix[0] = 1.0f;
	viewMatrix[1] = 0.0f;
	viewMatrix[2] = -0.0f;
	viewMatrix[3] = 0.0f;
	
	viewMatrix[4] = 0.0f;
	viewMatrix[5] = 1.0f;
	viewMatrix[6] = -0.0f;
	viewMatrix[7] = 0.0f;
	
	viewMatrix[8] = 0.0f;
	viewMatrix[9] = 0.0f;
	viewMatrix[10] = -1.0f;
	viewMatrix[11] = 0.0f;
	
	viewMatrix[12] = x;
	viewMatrix[13] = y;
	viewMatrix[14] = -z;
	viewMatrix[15] = 1.0f;


}

void Render::updateViewMatrix(GLfloat *modelView) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		/* ( 1  0  0 -(1*x + 0*y + 0*z))
		   ( 0  1  0 -(0*x + 1*y + 0*z))
		   (-0 -0 -1 -(0*x + 0*y + 1*z))
		   ( 0  0  0                  1) */	
		worldMatrix[0] = viewMatrix[0];
		worldMatrix[1] = viewMatrix[1];
		worldMatrix[2] = viewMatrix[2];
		worldMatrix[3] = viewMatrix[3];
		worldMatrix[4] = viewMatrix[4];
		worldMatrix[5] = viewMatrix[5];
		worldMatrix[6] = viewMatrix[6];
		worldMatrix[7] = viewMatrix[7];
		worldMatrix[8] = viewMatrix[8];
		worldMatrix[9] = viewMatrix[9];
		worldMatrix[10] = viewMatrix[10];
		worldMatrix[11] = viewMatrix[11];
		worldMatrix[12] = viewMatrix[12];
		worldMatrix[13] = viewMatrix[13];
		worldMatrix[14] = viewMatrix[14];
		worldMatrix[15] = viewMatrix[15];
	invertMatrix(worldMatrix);
	glLoadMatrixf(worldMatrix);
	glMultMatrixf(modelView);
}

void Render::invertMatrix(float *m){
	float worldMatrix[16]={
						/*inverting view matrix before giving it to opengl,
						transpose axis vectors and negate translation values since
						matrix is homogenous*/
						  m[0], 
						  m[4], 
						  -m[8], 
						  0,
						  
						  m[1], 
						  m[5], 
						  -m[9], 
						  0,
						  
						  m[2], 
						  m[6], 
						  -m[10], 
						  0,

						  -(m[0]*m[12] + m[1]*m[13] + m[2]*m[14]),
						  -(m[4]*m[12] + m[5]*m[13] + m[6]*m[14]),
						  (m[8]*m[12] + m[9]*m[13] + m[10]*m[14]), 
						  1};
	m[0] = worldMatrix[0];
	m[1] = worldMatrix[1];
	m[2] = worldMatrix[2];
	m[3] = worldMatrix[3];
	m[4] = worldMatrix[4];
	m[5] = worldMatrix[5];
	m[6] = worldMatrix[6];
	m[7] = worldMatrix[7];
	m[8] = worldMatrix[8];
	m[9] = worldMatrix[9];
	m[10] = worldMatrix[10];
	m[11] = worldMatrix[11];
	m[12] = worldMatrix[12];
	m[13] = worldMatrix[13];
	m[14] = worldMatrix[14];
	m[15] = worldMatrix[15];
}

void Render::roll(float angle) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glLoadMatrixf(viewMatrix);
	//opengl can rotate for us
	glRotatef(angle, 0.0f,0.0f,1.0f);
	//store transformed matrix back into viewMatrix
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
	glPopMatrix();
}

void Render::yaw(float angle) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glLoadMatrixf(viewMatrix);
	//opengl can rotate for us
	glRotatef(angle, 0.0f,1.0f,0.0f);
	//store transformed matrix back into viewMatrix
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
	glPopMatrix();
}

void Render::pitch(float angle) {
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//	glLoadMatrixf(viewMatrix);
//	//opengl can rotate for us
	glMatrixMode(GL_MODELVIEW);
	glRotatef(angle, 0.0f,1.0f,0.0f);
//	//store transformed matrix back into viewMatrix
//	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
//	glPopMatrix();
}

void Render::zoom(GLfloat zoom){
	viewMatrix[12] += viewMatrix[8]*zoom;
	viewMatrix[13] += viewMatrix[9]*zoom;
	viewMatrix[14] += viewMatrix[10]*zoom;
}
